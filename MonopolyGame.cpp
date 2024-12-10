#include "MonopolyGame.h"
#include "Player.h"
#include "Board.h"
#include "Bank.h"
#include <iostream>
#include <limits>

MonopolyGame::MonopolyGame() 
    : bank(std::make_unique<Bank>()),
      board(std::make_unique<Board>(*bank)),
      gameOver(false),
      currentPlayerIndex(0) {
    
    // Add this line after creating both bank and board
    bank->setBoard(board.get());
    
    initializePlayers();
}

void MonopolyGame::initializePlayers() {
    int numPlayers;
    std::string input;
    bool validInput = false;
    
    std::cout << "Welcome to Monopoly!\n\n";
    
    while (!validInput) {
        std::cout << "Enter number of players (2-8): ";
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        // Check if input is a number
        bool isNumber = true;
        for (char c : input) {
            if (!isdigit(c)) {
                isNumber = false;
                break;
            }
        }
        
        if (!isNumber) {
            std::cout << "Error: Please enter a valid number.\n";
            continue;
        }
        
        // Convert to integer and check range
        numPlayers = std::stoi(input);
        if (numPlayers < 2 || numPlayers > 8) {
            std::cout << "Error: Number of players must be between 2 and 8.\n";
            continue;
        }
        
        validInput = true;
    }

    for (int i = 0; i < numPlayers; i++) {
        std::string name;
        std::cout << "Enter name for Player " << (i + 1) << ": ";
        std::getline(std::cin, name);
        
        if (name.empty()) {
            std::cout << "Error: Name cannot be empty.\n";
            i--;  // Retry if empty name
            continue;
        }
        
        players.push_back(std::make_unique<Player>(name));
    }
}

void MonopolyGame::playGame() {
    while (!gameOver) {
        playTurn();
        if (players.size() == 1) {
            announceWinner();
            break;
        }
    }
}
void MonopolyGame::playTurn() {
    Player& currentPlayer = *players[currentPlayerIndex];
    std::cout << "\n" << currentPlayer.getName() << "'s turn" << std::endl;
    std::cout << "Current money: $" << currentPlayer.getMoney() << std::endl;

    // Remove the input buffer clearing from here since we'll do it in handleRegularTurn

    if (currentPlayer.isInJail()) {
        handleJailTurn(currentPlayer);
    } else {
        handleRegularTurn(currentPlayer);
    }

    handlePostTurnActions(currentPlayer);
    nextPlayer();
}

void MonopolyGame::handleJailTurn(Player& player) {
    while (true) {  // Keep asking until valid input received
        std::cout << "You are in Jail!" << std::endl;
        std::cout << "1. Pay $50 fine" << std::endl;
        std::cout << "2. Try to roll doubles" << std::endl;
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();  // Clear error flags
            std::cin.ignore(100, '\n');  // Clear input buffer
            std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
            continue;  // Ask again
        }
        std::cin.ignore(100, '\n');  // Clear input buffer

        if (choice == 1) {
            if (player.canAfford(50)) {
                player -= 50;
                player.setInJail(false);
                handleRegularTurn(player);
                break;  // Exit the loop after handling the turn
            } else {
                std::cout << "You can't afford the fine! You must try to roll doubles." << std::endl;
                choice = 2;  // Force roll doubles option
            }
        }
        
        if (choice == 2) {
            auto [roll1, roll2] = dice.roll();
            std::cout << "Rolled: " << roll1 << " + " << roll2 << std::endl;
            if (dice.isDoubles() || player.tryToLeaveJail()) {
                player.setInJail(false);
                board->movePlayer(player, roll1 + roll2);
            }
            break;  // Exit the loop after handling the roll
        } 
        
        if (choice != 1 && choice != 2) {
            std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
            continue;  // Ask again
        }
    }
}

void MonopolyGame::handleRegularTurn(Player& player) {
    do {
        std::cout << "Press Enter to roll dice...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        auto [roll1, roll2] = dice.roll();
        std::cout << "Rolled: " << roll1 << " + " << roll2 << " = " << (roll1 + roll2) << std::endl;
        
        board->movePlayer(player, roll1 + roll2);
        
        if (dice.isDoubles()) {
            if (dice.getDoublesCount() == 3) {
                std::cout << "Three doubles! Go to Jail!" << std::endl;
                player.goToJail();
                return;
            }
            std::cout << "Doubles! Roll again!" << std::endl;
        }
    } while (dice.isDoubles() && !player.isInJail());
}

void MonopolyGame::handlePostTurnActions(Player& player) {
    while (true) {
        std::cout << "\nActions:" << std::endl;
        std::cout << "1. View/Manage Properties" << std::endl;
        std::cout << "2. Trade" << std::endl;
        std::cout << "3. Build Houses/Hotels" << std::endl;
        std::cout << "4. Mortgage Properties" << std::endl;
        std::cout << "5. End Turn" << std::endl;
        std::cout << "6. Exit Game" << std::endl;
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: manageProperties(player); break;
            case 2: handleTrade(player); break;
            case 3: handleDevelopment(player); break;
            case 4: handleMortgage(player); break;
            case 5: return;
            case 6: 
                std::cout << "Are you sure you want to exit? (y/n): ";
                char confirm;
                std::cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    std::cout << "Thanks for playing!\n";
                    exit(0);  // This will immediately exit the program
                }
                break;
            default: std::cout << "Invalid choice" << std::endl;
        }
    }
}

void MonopolyGame::manageProperties(Player& player) {
    const auto& properties = player.getProperties();
    if (properties.empty()) {
        std::cout << "You don't own any properties." << std::endl;
        return;
    }

    std::cout << "\nYour Properties:" << std::endl;
    for (size_t i = 0; i < properties.size(); i++) {
        std::cout << i + 1 << ". " << *properties[i] << std::endl;
    }
}

void MonopolyGame::handleTrade(Player& player) {
    if (players.size() < 2) {
        std::cout << "No players available to trade with." << std::endl;
        return;
    }

    std::cout << "Select player to trade with:" << std::endl;
    for (size_t i = 0; i < players.size(); i++) {
        if (i != currentPlayerIndex) {
            std::cout << i + 1 << ". " << players[i]->getName() << std::endl;
        }
    }
    
    int playerChoice;
    std::cin >> playerChoice;
    if (playerChoice < 1 || playerChoice > static_cast<int>(players.size())) {
        std::cout << "Invalid player selection" << std::endl;
        return;
    }

    std::cout << "Trading not implemented in this version" << std::endl;
}

void MonopolyGame::handleDevelopment(Player& player) {
    const auto& properties = player.getProperties();
    std::vector<std::shared_ptr<StandardProperty>> developableProps;
    
    for (const auto& prop : properties) {
        if (auto stdProp = std::dynamic_pointer_cast<StandardProperty>(prop)) {
            if (player.canDevelopProperty(prop)) {
                developableProps.push_back(stdProp);
            }
        }
    }

    if (developableProps.empty()) {
        std::cout << "No properties available for development." << std::endl;
        return;
    }

    std::cout << "Select property to develop:" << std::endl;
    for (size_t i = 0; i < developableProps.size(); i++) {
        std::cout << i + 1 << ". " << *developableProps[i] << std::endl;
    }

    int choice;
    std::cin >> choice;
    if (choice < 1 || choice > static_cast<int>(developableProps.size())) {
        std::cout << "Invalid choice" << std::endl;
        return;
    }

    auto prop = developableProps[choice - 1];
    if (bank->buyHouse(player, prop)) {
        std::cout << "Development successful!" << std::endl;
    } else {
        std::cout << "Development failed!" << std::endl;
    }
}

void MonopolyGame::handleMortgage(Player& player) {
    const auto& properties = player.getProperties();
    if (properties.empty()) {
        std::cout << "No properties available to mortgage." << std::endl;
        return;
    }

    std::cout << "\nSelect property to mortgage/unmortgage:" << std::endl;
    for (size_t i = 0; i < properties.size(); i++) {
        std::cout << i + 1 << ". " << *properties[i] 
                  << (properties[i]->isMortgaged() ? " (Mortgaged)" : "") << std::endl;
    }

    int choice;
    std::cin >> choice;
    if (choice < 1 || choice > static_cast<int>(properties.size())) {
        std::cout << "Invalid choice" << std::endl;
        return;
    }

    auto prop = properties[choice - 1];
    if (prop->isMortgaged()) {
        if (bank->unmortgageProperty(player, prop)) {
            std::cout << "Property unmortgaged successfully!" << std::endl;
        } else {
            std::cout << "Failed to unmortgage property!" << std::endl;
        }
    } else {
        if (bank->mortgageProperty(player, prop)) {
            std::cout << "Property mortgaged successfully!" << std::endl;
        } else {
            std::cout << "Failed to mortgage property!" << std::endl;
        }
    }
}

void MonopolyGame::handleBankruptcy(Player& player) {
    std::cout << player.getName() << " is bankrupt!" << std::endl;
    bank->handleBankruptcy(player);
    
    players.erase(players.begin() + currentPlayerIndex);
    
    if (currentPlayerIndex == players.size()) {
        currentPlayerIndex = 0;
    }
}

void MonopolyGame::nextPlayer() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

void MonopolyGame::announceWinner() {
    std::cout << "\nGame Over!" << std::endl;
    std::cout << players[0]->getName() << " wins with $" 
              << players[0]->getMoney() << "!" << std::endl;
}