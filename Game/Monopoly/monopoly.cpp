#include "monopoly.h"

// Main function
int main() {
    srand(static_cast<unsigned int>(time(0)));
    
    Property board[BOARD_SIZE];
    GameState gameState;
    gameState.board = board;
    char choice;
    bool exitProgram = false;
    
    while (!exitProgram) {
        cout << "=== Welcome to Monopoly! ===\n\n";
        cout << "1. Load Saved Game\n";
        cout << "2. Start New Game\n";
        cout << "3. Exit\n";
        cout << "Choose an option (1-3): ";
        cin >> choice;
        
        switch (choice) {
            case '1': {
                if (!loadGame(&gameState, board)) {
                    cout << "Would you like to start a new game instead? (y/n): ";
                    cin >> choice;
                    if (choice != 'y' && choice != 'Y') {
                        continue;  // Go back to main menu
                    }
                    initializeBoard(board);
                    initializeCards(&gameState);
                    initializePlayers(&gameState);
                }
                
                gameState.gameOver = false;  // Reset game over flag
                // Game loop
                while (!gameState.gameOver) {
                    displayGameState(&gameState, board);
                    processPlayerTurn(&gameState, board);
                    
                    if (checkWinCondition(&gameState)) {
                        gameState.gameOver = true;
                        continue;
                    }
                    
                    cout << "\nContinue playing? (y/n): ";
                    cin >> choice;
                    if (choice != 'y' && choice != 'Y') {
                        cout << "Would you like to save the game? (y/n): ";
                        cin >> choice;
                        if (choice == 'y' || choice == 'Y') {
                            saveGame(&gameState, board);
                        }
                        break;
                    }
                }
                break;
            }
            
            case '2': {
                // Initialize new game
                initializeBoard(board);
                initializeCards(&gameState);
                initializePlayers(&gameState);
                
                gameState.gameOver = false;  // Reset game over flag
                // Game loop
                while (!gameState.gameOver) {
                    displayGameState(&gameState, board);
                    processPlayerTurn(&gameState, board);
                    
                    if (checkWinCondition(&gameState)) {
                        gameState.gameOver = true;
                        continue;
                    }
                    
                    cout << "\nContinue playing? (y/n): ";
                    cin >> choice;
                    if (choice != 'y' && choice != 'Y') {
                        cout << "Would you like to save the game? (y/n): ";
                        cin >> choice;
                        if (choice == 'y' || choice == 'Y') {
                            saveGame(&gameState, board);
                        }
                        break;
                    }
                }
                break;
            }
            
            case '3': {
                exitProgram = true;
                cout << "\nThanks for playing Monopoly!\n";
                break;
            }
            
            default: {
                cout << "Invalid choice. Please try again.\n\n";
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }
        }
        
        if (!exitProgram && !gameState.gameOver) {
            cout << "\nWould you like to return to the main menu? (y/n): ";
            cin >> choice;
            if (choice != 'y' && choice != 'Y') {
                exitProgram = true;
                cout << "\nThanks for playing Monopoly!\n";
            }
            cout << "\n";
        }
    }
    
    return 0;
}
// Initialization Functions
void initializeBoard(Property board[]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        strcpy(board[i].name, "");
        board[i].type = 0;
        board[i].color = -1;
        board[i].price = 0;
        board[i].baseRent = 0;
        board[i].owner = -1;
        board[i].mortgaged = false;
    }

    // GO
    strcpy(board[0].name, "GO");
    board[0].type = 0;  // SPECIAL
    
    // Brown Properties
    strcpy(board[1].name, "Mediterranean Avenue");
    board[1].type = 1;
    board[1].color = 0;
    board[1].price = 60;
    
    strcpy(board[2].name, "Community Chest");
    board[2].type = 5;
    
    strcpy(board[3].name, "Baltic Avenue");
    board[3].type = 1;
    board[3].color = 0;
    board[3].price = 60;
    
    strcpy(board[4].name, "Income Tax");
    board[4].type = 3;
    board[4].price = 200;
    
    strcpy(board[5].name, "Reading Railroad");
    board[5].type = 2;
    board[5].price = 200;
    
    strcpy(board[6].name, "Oriental Avenue");
    board[6].type = 1;
    board[6].color = 1;
    board[6].price = 100;
    
    strcpy(board[7].name, "Chance");
    board[7].type = 4;
    
    strcpy(board[8].name, "Vermont Avenue");
    board[8].type = 1;
    board[8].color = 1;
    board[8].price = 100;
    
    strcpy(board[9].name, "Connecticut Avenue");
    board[9].type = 1;
    board[9].color = 1;
    board[9].price = 120;
    
    strcpy(board[10].name, "Jail");
    board[10].type = 0;
    
    strcpy(board[11].name, "St. Charles Place");
    board[11].type = 1;
    board[11].color = 2;
    board[11].price = 140;
    
    strcpy(board[12].name, "Electric Company");
    board[12].type = 3;
    board[12].price = 150;
    
    strcpy(board[13].name, "States Avenue");
    board[13].type = 1;
    board[13].color = 2;
    board[13].price = 140;
    
    strcpy(board[14].name, "Virginia Avenue");
    board[14].type = 1;
    board[14].color = 2;
    board[14].price = 160;
    
    strcpy(board[15].name, "Pennsylvania Railroad");
    board[15].type = 2;
    board[15].price = 200;
    
    // Continue with the rest of the properties...
    
    strcpy(board[20].name, "Free Parking");
    board[20].type = 0;
    
    strcpy(board[30].name, "Go to Jail");
    board[30].type = 0;
    
    // Initialize all property values
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i].type == 1) {  // Regular properties
            board[i].baseRent = board[i].price / 10;
            board[i].rentWithSet = board[i].baseRent * 2;
            board[i].houseCost = board[i].price / 2;
            board[i].houses = 0;
            
            // Set house rents
            for (int j = 0; j < 4; j++) {
                board[i].rentWithHouses[j] = board[i].baseRent * (j + 3);
            }
            board[i].rentWithHotel = board[i].baseRent * 12;
        }
        else if (board[i].type == 2) {  // Railroads
            board[i].baseRent = 25;
        }
    }
}

void initializeCards(GameState* game) {
    // Initialize Chance Cards
    strcpy(game->chanceCards[0].text, "Advance to GO");
    game->chanceCards[0].actionType = 0;
    game->chanceCards[0].actionValue = 0;

    strcpy(game->chanceCards[1].text, "Bank pays dividend of $50");
    game->chanceCards[1].actionType = 1;
    game->chanceCards[1].actionValue = 50;

    strcpy(game->chanceCards[2].text, "Pay poor tax of $15");
    game->chanceCards[2].actionType = 1;
    game->chanceCards[2].actionValue = -15;

    strcpy(game->chanceCards[3].text, "Get Out of Jail Free");
    game->chanceCards[3].actionType = 2;
    game->chanceCards[3].actionValue = 0;

    // Initialize Community Chest Cards
    strcpy(game->communityCards[0].text, "Bank error in your favor - Collect $200");
    game->communityCards[0].actionType = 1;
    game->communityCards[0].actionValue = 200;

    strcpy(game->communityCards[1].text, "Doctor's fees - Pay $50");
    game->communityCards[1].actionType = 1;
    game->communityCards[1].actionValue = -50;

    strcpy(game->communityCards[2].text, "Get Out of Jail Free");
    game->communityCards[2].actionType = 2;
    game->communityCards[2].actionValue = 0;

    game->chanceIndex = 0;
    game->communityIndex = 0;
}

void initializePlayers(GameState* game) {
    cout << "Enter number of players (2-4): ";
    cin >> game->numPlayers;
    
    while (game->numPlayers < 2 || game->numPlayers > MAX_PLAYERS) {
        cout << "Invalid number. Please enter 2-4 players: ";
        cin >> game->numPlayers;
    }

    cin.ignore(); // Clear the input buffer
    
    for (int i = 0; i < game->numPlayers; i++) {
        cout << "Enter name for Player " << (i + 1) << ": ";
        cin.getline(game->players[i].name, MAX_NAME_LENGTH);
        
        game->players[i].money = STARTING_MONEY;
        game->players[i].position = 0;
        game->players[i].inJail = false;
        game->players[i].jailTurns = 0;
        game->players[i].bankrupt = false;
        game->players[i].getOutOfJailCards = 0;
        game->players[i].propertyCount = 0;
        
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->players[i].ownedProperties[j] = -1;
        }
    }

    game->currentPlayer = 0;
    game->gameOver = false;
}

void rollDice(int* dice1, int* dice2) {
    *dice1 = (rand() % 6) + 1;
    *dice2 = (rand() % 6) + 1;
}

bool isDouble(int dice1, int dice2) {
    return dice1 == dice2;
}
// Movement and Property Handling Functions
void movePlayer(GameState* game, Property board[], int totalSpaces) {
    Player* currentPlayer = &game->players[game->currentPlayer];
    
    // Calculate new position
    int newPosition = (currentPlayer->position + totalSpaces) % BOARD_SIZE;
    
    // Check if passing GO
    if (newPosition < currentPlayer->position) {
        currentPlayer->money += 200;
        cout << currentPlayer->name << " passed GO! Collect $200\n";
    }
    
    currentPlayer->position = newPosition;
    
    // Add error checking for property names
    if (strlen(board[newPosition].name) > 0) {
        cout << currentPlayer->name << " landed on " << board[newPosition].name << endl;
    } else {
        cout << currentPlayer->name << " landed on space " << newPosition << endl;
    }
}

int calculateRent(const Property& property, const GameState* game, int diceRoll, int propertyIndex) {
    if (property.mortgaged) return 0;
    
    switch (property.type) {
        case 1: // REGULAR_PROPERTY
            if (property.houses == 0) {
                // Pass the board parameter directly instead of using game->board
                if (hasMonopoly(game, game->board, property.owner, propertyIndex)) {
                    return property.rentWithSet;
                }
                return property.baseRent;
            } else if (property.houses == HOTEL) {
                return property.rentWithHotel;
            } else {
                return property.rentWithHouses[property.houses - 1];
            }
            
        case 2: // RAILROAD
            // Pass board parameter directly
            return property.baseRent * 
                   (1 << (countRailroadsOwned(game, game->board, property.owner) - 1));
            
        case 3: // UTILITY
            // Pass board parameter directly
            if (countUtilitiesOwned(game, game->board, property.owner) == 1) {
                return diceRoll * 4;
            } else {
                return diceRoll * 10;
            }
            
        default:
            return 0;
    }
}

// Display Functions
void displayProperty(const Property& prop) {
    cout << "\nProperty: " << prop.name;
    
    if (prop.type == 1) { // REGULAR_PROPERTY
        cout << "\nPrice: $" << prop.price
             << "\nRent: $" << prop.baseRent
             << "\nRent with set: $" << prop.rentWithSet
             << "\nHouses: " << prop.houses
             << "\nMortgaged: " << (prop.mortgaged ? "Yes" : "No");
    } else if (prop.type == 2) { // RAILROAD
        cout << "\nPrice: $" << prop.price
             << "\nBase Rent: $" << prop.baseRent;
    } else if (prop.type == 3) { // UTILITY
        cout << "\nPrice: $" << prop.price
             << "\nRent is 4x or 10x dice roll";
    }
    cout << endl;
}

void displayPlayerProperties(const GameState* game, const Property board[], int playerNum) {
    const Player* player = &game->players[playerNum];
    cout << "\nProperties owned by " << player->name << ":\n";
    
    bool hasProperties = false;
    for (int i = 0; i < player->propertyCount; i++) {
        if (player->ownedProperties[i] != -1) {
            hasProperties = true;
            cout << i + 1 << ". ";
            displayProperty(board[player->ownedProperties[i]]);
        }
    }
    
    if (!hasProperties) {
        cout << "No properties owned.\n";
    }
}

void displayGameState(const GameState* game, const Property board[]) {
    cout << "\n=== Current Game State ===\n";
    for (int i = 0; i < game->numPlayers; i++) {
        const Player* player = &game->players[i];
        cout << player->name 
             << "\nMoney: $" << player->money
             << "\nPosition: " << board[player->position].name
             << "\nGet Out of Jail Cards: " << player->getOutOfJailCards
             << "\nIn Jail: " << (player->inJail ? "Yes" : "No")
             << "\nBankrupt: " << (player->bankrupt ? "Yes" : "No")
             << "\n\n";
    }
}

void displayMenu() {
    cout << "\n=== MENU ===\n"
         << "1. Roll Dice\n"
         << "2. View Properties\n"
         << "3. Trade Properties\n"
         << "4. Build Houses\n"
         << "5. Sell Houses\n"
         << "6. Mortgage Property\n"
         << "7. Unmortgage Property\n"
         << "8. Save Game\n"
         << "9. Quit\n"
         << "Choice: ";
}

// Property Ownership Functions
bool hasMonopoly(const GameState* game, const Property board[], int playerNum, int propertyIndex) {
    int color = board[propertyIndex].color;
    int colorCount = 0;
    int playerCount = 0;
    
    // Count properties of this color
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i].color == color) {
            colorCount++;
            if (board[i].owner == playerNum) {
                playerCount++;
            }
        }
    }
    
    return (colorCount > 0 && colorCount == playerCount);
}

int countRailroadsOwned(const GameState* game, const Property board[], int playerNum) {
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i].type == 2 && board[i].owner == playerNum) { // RAILROAD
            count++;
        }
    }
    return count;
}

int countUtilitiesOwned(const GameState* game, const Property board[], int playerNum) {
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i].type == 3 && board[i].owner == playerNum) { // UTILITY
            count++;
        }
    }
    return count;
}

bool canAffordProperty(const Player& player, const Property& property) {
    return player.money >= property.price;
}

void handleProperty(GameState* game, Property board[], int diceRoll) {
    Player* currentPlayer = &game->players[game->currentPlayer];
    Property* currentProperty = &board[currentPlayer->position];
    
    if (currentProperty->type == 1 || // REGULAR_PROPERTY
        currentProperty->type == 2 || // RAILROAD
        currentProperty->type == 3) { // UTILITY
        
        if (currentProperty->owner == -1) {
            if (canAffordProperty(*currentPlayer, *currentProperty)) {
                char choice;
                cout << "Would you like to buy " << currentProperty->name 
                     << " for $" << currentProperty->price << "? (y/n): ";
                cin >> choice;
                
                if (choice == 'y' || choice == 'Y') {
                    currentPlayer->money -= currentProperty->price;
                    currentProperty->owner = game->currentPlayer;
                    currentPlayer->ownedProperties[currentPlayer->propertyCount++] = currentPlayer->position;
                    cout << "Property purchased successfully!\n";
                }
            } else {
                cout << "Not enough money to purchase this property.\n";
            }
        } else if (currentProperty->owner != game->currentPlayer) {
            int rentAmount = calculateRent(*currentProperty, game, diceRoll, currentPlayer->position);
            currentPlayer->money -= rentAmount;
            game->players[currentProperty->owner].money += rentAmount;
            cout << currentPlayer->name << " paid $" << rentAmount << " in rent to " 
                 << game->players[currentProperty->owner].name << endl;
        }
    }
}
// Trading and Property Management Functions

void tradeProperties(GameState* game, Property board[]) {
    int player1 = game->currentPlayer;
    cout << "Enter player number to trade with (";
    for (int i = 0; i < game->numPlayers; i++) {
        if (i != player1) cout << i + 1 << " ";
    }
    cout << "): ";
    
    int player2;
    cin >> player2;
    player2--; // Convert to 0-based index
    
    if (player2 < 0 || player2 >= game->numPlayers || player2 == player1) {
        cout << "Invalid player selection!\n";
        return;
    }
    
    // Display properties of both players
    cout << "\nYour properties:\n";
    displayPlayerProperties(game, board, player1);
    cout << "\n" << game->players[player2].name << "'s properties:\n";
    displayPlayerProperties(game, board, player2);
    
    // Get property selections
    int prop1 = -1, prop2 = -1;
    int money1 = 0, money2 = 0;
    
    cout << "\nEnter property number to offer (0 for none): ";
    cin >> prop1;
    if (prop1 > 0) prop1--;
    
    cout << "Enter amount of money to offer (0 for none): $";
    cin >> money1;
    
    cout << "\nEnter property number to receive (0 for none): ";
    cin >> prop2;
    if (prop2 > 0) prop2--;
    
    cout << "Enter amount of money to receive (0 for none): $";
    cin >> money2;
    
    // Validate selections
    if ((prop1 >= 0 && (prop1 >= game->players[player1].propertyCount || 
        game->players[player1].ownedProperties[prop1] == -1)) ||
        (prop2 >= 0 && (prop2 >= game->players[player2].propertyCount || 
        game->players[player2].ownedProperties[prop2] == -1))) {
        cout << "Invalid property selection!\n";
        return;
    }
    
    // Check if players have enough money
    if (game->players[player1].money < money1 || 
        game->players[player2].money < money2) {
        cout << "One or both players don't have enough money!\n";
        return;
    }
    
    // Ask other player to accept
    cout << "\n" << game->players[player2].name << ", do you accept this trade? (y/n): ";
    char choice;
    cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        // Execute trade
        if (prop1 >= 0) {
            int propertyIndex = game->players[player1].ownedProperties[prop1];
            board[propertyIndex].owner = player2;
            // Add to player2's properties
            game->players[player2].ownedProperties[game->players[player2].propertyCount++] = propertyIndex;
            // Remove from player1's properties
            game->players[player1].ownedProperties[prop1] = -1;
        }
        
        if (prop2 >= 0) {
            int propertyIndex = game->players[player2].ownedProperties[prop2];
            board[propertyIndex].owner = player1;
            // Add to player1's properties
            game->players[player1].ownedProperties[game->players[player1].propertyCount++] = propertyIndex;
            // Remove from player2's properties
            game->players[player2].ownedProperties[prop2] = -1;
        }
        
        // Exchange money
        game->players[player1].money -= money1;
        game->players[player2].money += money1;
        game->players[player2].money -= money2;
        game->players[player1].money += money2;
        
        cout << "Trade completed successfully!\n";
    } else {
        cout << "Trade rejected.\n";
    }
}

void mortgageProperty(GameState* game, Property board[], int propertyIndex) {
    Player* currentPlayer = &game->players[game->currentPlayer];
    Property* property = &board[propertyIndex];
    
    if (property->owner != game->currentPlayer) {
        cout << "You don't own this property!\n";
        return;
    }
    
    if (property->mortgaged) {
        cout << "This property is already mortgaged!\n";
        return;
    }
    
    if (property->houses > 0) {
        cout << "You must sell all houses first!\n";
        return;
    }
    
    property->mortgaged = true;
    currentPlayer->money += property->price / 2;
    cout << "Property mortgaged. Received $" << (property->price / 2) << endl;
}

void unmortgageProperty(GameState* game, Property board[], int propertyIndex) {
    Player* currentPlayer = &game->players[game->currentPlayer];
    Property* property = &board[propertyIndex];
    int unmortgageCost = (property->price / 2) * 1.1; // 10% interest
    
    if (property->owner != game->currentPlayer) {
        cout << "You don't own this property!\n";
        return;
    }
    
    if (!property->mortgaged) {
        cout << "This property is not mortgaged!\n";
        return;
    }
    
    if (currentPlayer->money < unmortgageCost) {
        cout << "You don't have enough money to unmortgage this property!\n";
        return;
    }
    
    property->mortgaged = false;
    currentPlayer->money -= unmortgageCost;
    cout << "Property unmortgaged. Paid $" << unmortgageCost << endl;
}

void buildHouse(GameState* game, Property board[], int propertyIndex) {
    Player* currentPlayer = &game->players[game->currentPlayer];
    Property* property = &board[propertyIndex];
    
    if (property->type != 1) { // Not a REGULAR_PROPERTY
        cout << "Can only build houses on regular properties!\n";
        return;
    }
    
    if (!hasMonopoly(game, board, game->currentPlayer, propertyIndex)) {
        cout << "You need to own all properties of this color to build houses!\n";
        return;
    }
    
    if (property->houses >= HOTEL) {
        cout << "Already has a hotel! Cannot build more.\n";
        return;
    }
    
    if (currentPlayer->money < property->houseCost) {
        cout << "Not enough money to build a house!\n";
        return;
    }
    
    // Check for even building
    int color = property->color;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i].color == color && board[i].houses < property->houses) {
            cout << "Must build evenly across properties of the same color!\n";
            return;
        }
    }
    
    property->houses++;
    currentPlayer->money -= property->houseCost;
    
    if (property->houses == HOTEL) {
        cout << "Built a hotel on " << property->name << endl;
    } else {
        cout << "Built house #" << property->houses << " on " << property->name << endl;
    }
}

void sellHouse(GameState* game, Property board[], int propertyIndex) {
    Player* currentPlayer = &game->players[game->currentPlayer];
    Property* property = &board[propertyIndex];
    
    if (property->owner != game->currentPlayer) {
        cout << "You don't own this property!\n";
        return;
    }
    
    if (property->houses == 0) {
        cout << "No houses to sell!\n";
        return;
    }
    
    // Check for even selling
    int color = property->color;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i].color == color && board[i].houses > property->houses) {
            cout << "Must sell houses evenly across properties of the same color!\n";
            return;
        }
    }
    
    property->houses--;
    currentPlayer->money += property->houseCost / 2;
    
    if (property->houses == 4) {
        cout << "Sold hotel back to houses on " << property->name << endl;
    } else {
        cout << "Sold house on " << property->name << endl;
    }
    cout << "Received $" << (property->houseCost / 2) << endl;
}
// Special Space Handling Functions

void handleSpecialSpace(GameState* game, Property board[], int position) {
    Player* currentPlayer = &game->players[game->currentPlayer];
    
    switch (board[position].type) {
        case 3: // TAX
            if (position == 4) { // Income Tax
                currentPlayer->money -= 200;
                cout << currentPlayer->name << " paid $200 in Income Tax\n";
            } else if (position == 38) { // Luxury Tax
                currentPlayer->money -= 100;
                cout << currentPlayer->name << " paid $100 in Luxury Tax\n";
            }
            break;
            
        case 4: // CHANCE
            handleChance(game, board);
            break;
            
        case 5: // COMMUNITY_CHEST
            handleCommunityChest(game, board);
            break;
            
        case 0: // SPECIAL
            if (position == 30) { // Go to Jail
                goToJail(game);
            }
            break;
    }
}

void handleChance(GameState* game, Property board[]) {
    Player* currentPlayer = &game->players[game->currentPlayer];
    Card currentCard = game->chanceCards[game->chanceIndex];
    
    cout << "\nChance Card: " << currentCard.text << endl;
    
    switch (currentCard.actionType) {
        case 0: // Move
            if (currentCard.actionValue < currentPlayer->position) {
                currentPlayer->money += 200;
                cout << "Passed GO! Collect $200\n";
            }
            currentPlayer->position = currentCard.actionValue;
            cout << "Moved to " << board[currentPlayer->position].name << endl;
            break;
            
        case 1: // Money change
            currentPlayer->money += currentCard.actionValue;
            if (currentCard.actionValue > 0) {
                cout << "Collected $" << currentCard.actionValue << endl;
            } else {
                cout << "Paid $" << -currentCard.actionValue << endl;
            }
            break;
            
        case 2: // Get out of jail free
            currentPlayer->getOutOfJailCards++;
            cout << "Received Get Out of Jail Free card\n";
            break;
    }
    
    game->chanceIndex = (game->chanceIndex + 1) % 16;
}

void handleCommunityChest(GameState* game, Property board[]) {
    Player* currentPlayer = &game->players[game->currentPlayer];
    Card currentCard = game->communityCards[game->communityIndex];
    
    cout << "\nCommunity Chest Card: " << currentCard.text << endl;
    
    switch (currentCard.actionType) {
        case 0: // Move
            if (currentCard.actionValue < currentPlayer->position) {
                currentPlayer->money += 200;
                cout << "Passed GO! Collect $200\n";
            }
            currentPlayer->position = currentCard.actionValue;
            cout << "Moved to " << board[currentPlayer->position].name << endl;
            break;
            
        case 1: // Money change
            currentPlayer->money += currentCard.actionValue;
            if (currentCard.actionValue > 0) {
                cout << "Collected $" << currentCard.actionValue << endl;
            } else {
                cout << "Paid $" << -currentCard.actionValue << endl;
            }
            break;
            
        case 2: // Get out of jail free
            currentPlayer->getOutOfJailCards++;
            cout << "Received Get Out of Jail Free card\n";
            break;
    }
    
    game->communityIndex = (game->communityIndex + 1) % 16;
}

void goToJail(GameState* game) {
    Player* currentPlayer = &game->players[game->currentPlayer];
    currentPlayer->position = 10; // Jail position
    currentPlayer->inJail = true;
    currentPlayer->jailTurns = 0;
    cout << currentPlayer->name << " was sent to Jail!\n";
}

void handleJailTurn(GameState* game, Property board[]) {
    Player* currentPlayer = &game->players[game->currentPlayer];
    
    cout << "\nYou are in Jail! Turn " << (currentPlayer->jailTurns + 1) << " of 3\n"
         << "1. Pay $50 fine\n"
         << "2. Use Get Out of Jail Free card\n"
         << "3. Roll for doubles\n"
         << "Choice: ";
         
    int choice;
    cin >> choice;
    
    switch (choice) {
        case 1:
            if (currentPlayer->money >= 50) {
                currentPlayer->money -= 50;
                currentPlayer->inJail = false;
                currentPlayer->jailTurns = 0;
                cout << "Paid fine. You're out of jail!\n";
                
                // Regular turn
                int dice1, dice2;
                rollDice(&dice1, &dice2);
                cout << "Rolled: " << dice1 << " and " << dice2 << endl;
                movePlayer(game, board, dice1 + dice2);
                handleProperty(game, board, dice1 + dice2);
            } else {
                cout << "Not enough money to pay fine!\n";
            }
            break;
            
        case 2:
            if (currentPlayer->getOutOfJailCards > 0) {
                currentPlayer->getOutOfJailCards--;
                currentPlayer->inJail = false;
                currentPlayer->jailTurns = 0;
                cout << "Used Get Out of Jail Free card!\n";
                
                // Regular turn
                int dice1, dice2;
                rollDice(&dice1, &dice2);
                cout << "Rolled: " << dice1 << " and " << dice2 << endl;
                movePlayer(game, board, dice1 + dice2);
                handleProperty(game, board, dice1 + dice2);
            } else {
                cout << "No Get Out of Jail Free cards!\n";
            }
            break;
            
        case 3:
            int dice1, dice2;
            rollDice(&dice1, &dice2);
            cout << "Rolled: " << dice1 << " and " << dice2 << endl;
            
            if (isDouble(dice1, dice2)) {
                currentPlayer->inJail = false;
                currentPlayer->jailTurns = 0;
                cout << "Rolled doubles! You're out of jail!\n";
                movePlayer(game, board, dice1 + dice2);
                handleProperty(game, board, dice1 + dice2);
            } else {
                currentPlayer->jailTurns++;
                if (currentPlayer->jailTurns >= 3) {
                    currentPlayer->money -= 50;
                    currentPlayer->inJail = false;
                    currentPlayer->jailTurns = 0;
                    cout << "Third turn in jail. Paid $50 fine.\n";
                    movePlayer(game, board, dice1 + dice2);
                    handleProperty(game, board, dice1 + dice2);
                }
            }
            break;
    }
}

void handleBankruptcy(GameState* game, Property board[]) {
    Player* currentPlayer = &game->players[game->currentPlayer];
    
    // First try to mortgage all properties
    for (int i = 0; i < currentPlayer->propertyCount; i++) {
        if (currentPlayer->ownedProperties[i] != -1) {
            Property* prop = &board[currentPlayer->ownedProperties[i]];
            if (!prop->mortgaged) {
                mortgageProperty(game, board, currentPlayer->ownedProperties[i]);
            }
        }
    }
    
    // If still in debt, sell all houses
    if (currentPlayer->money < 0) {
        for (int i = 0; i < currentPlayer->propertyCount; i++) {
            if (currentPlayer->ownedProperties[i] != -1) {
                while (board[currentPlayer->ownedProperties[i]].houses > 0) {
                    sellHouse(game, board, currentPlayer->ownedProperties[i]);
                }
            }
        }
    }
    
    // If still in debt, declare bankruptcy
    if (currentPlayer->money < 0) {
        cout << currentPlayer->name << " has gone bankrupt!\n";
        currentPlayer->bankrupt = true;
        
        // Return all properties to bank
        for (int i = 0; i < currentPlayer->propertyCount; i++) {
            if (currentPlayer->ownedProperties[i] != -1) {
                Property* prop = &board[currentPlayer->ownedProperties[i]];
                prop->owner = -1;
                prop->houses = 0;
                prop->mortgaged = false;
            }
        }
        currentPlayer->propertyCount = 0;
    }
}

// Save/Load Functions
void saveGame(const GameState* game, const Property board[]) {
    ofstream outFile("monopoly_save.dat", ios::binary);
    if (!outFile) {
        cout << "Error opening save file!\n";
        return;
    }
    
    outFile.write(reinterpret_cast<const char*>(game), sizeof(GameState));
    outFile.write(reinterpret_cast<const char*>(board), sizeof(Property) * BOARD_SIZE);
    
    outFile.close();
    cout << "Game saved successfully!\n";
}

bool loadGame(GameState* game, Property board[]) {
    ifstream inFile("monopoly_save.dat", ios::binary);
    if (!inFile) {
        cout << "No saved game found.\n";
        return false;
    }
    
    try {
        inFile.read(reinterpret_cast<char*>(game), sizeof(GameState));
        inFile.read(reinterpret_cast<char*>(board), sizeof(Property) * BOARD_SIZE);
        
        inFile.close();
        cout << "Game loaded successfully!\n";
        return true;
    } catch (...) {
        inFile.close();
        cout << "Error loading game file.\n";
        return false;
    }
}

bool checkWinCondition(const GameState* game) {
    int activePlayers = 0;
    int lastActivePlayer = -1;
    
    for (int i = 0; i < game->numPlayers; i++) {
        if (!game->players[i].bankrupt) {
            activePlayers++;
            lastActivePlayer = i;
        }
    }
    
    if (activePlayers == 1) {
        cout << "\nGame Over! " << game->players[lastActivePlayer].name << " wins!\n";
        return true;
    }
    return false;
}
void processPlayerTurn(GameState* game, Property board[]) {
    Player* currentPlayer = &game->players[game->currentPlayer];
    
    if (currentPlayer->bankrupt) {
        game->currentPlayer = (game->currentPlayer + 1) % game->numPlayers;
        return;
    }

    cout << "\n=== " << currentPlayer->name << "'s turn ===\n";
    cout << "Current money: $" << currentPlayer->money << endl;
    cout << "Current position: " << board[currentPlayer->position].name << endl;
    
    int choice = 0;
    bool turnEnded = false;
    bool hasRolled = false;
    
    while (!turnEnded) {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: { // Roll Dice
                if (hasRolled) {
                    cout << "You've already rolled this turn!\n";
                    break;
                }
                
                if (currentPlayer->inJail) {
                    handleJailTurn(game, board);
                    turnEnded = true;
                } else {
                    int dice1, dice2;
                    rollDice(&dice1, &dice2);
                    cout << "\nRolled: " << dice1 << " and " << dice2 << endl;
                    
                    movePlayer(game, board, dice1 + dice2);
                    handleProperty(game, board, dice1 + dice2);
                    handleSpecialSpace(game, board, currentPlayer->position);
                    
                    hasRolled = true;
                    if (!isDouble(dice1, dice2)) {
                        turnEnded = true;
                    } else {
                        cout << "Doubles! You get another roll!\n";
                        hasRolled = false;
                    }
                }
                break;
            }
            
            case 2: // View Properties
                displayPlayerProperties(game, board, game->currentPlayer);
                break;
                
            case 3: // Trade
                if (!hasRolled) {
                    tradeProperties(game, board);
                } else {
                    cout << "Cannot trade after rolling dice!\n";
                }
                break;
                
            case 4: { // Build Houses
                if (!hasRolled) {
                    displayPlayerProperties(game, board, game->currentPlayer);
                    cout << "Enter property number to build house on (0 to cancel): ";
                    int propNum;
                    cin >> propNum;
                    if (propNum > 0) {
                        buildHouse(game, board, currentPlayer->ownedProperties[propNum - 1]);
                    }
                } else {
                    cout << "Cannot build houses after rolling dice!\n";
                }
                break;
            }
            
            case 5: { // Sell Houses
                if (!hasRolled) {
                    displayPlayerProperties(game, board, game->currentPlayer);
                    cout << "Enter property number to sell house from (0 to cancel): ";
                    int propNum;
                    cin >> propNum;
                    if (propNum > 0) {
                        sellHouse(game, board, currentPlayer->ownedProperties[propNum - 1]);
                    }
                } else {
                    cout << "Cannot sell houses after rolling dice!\n";
                }
                break;
            }
            
            case 6: { // Mortgage Property
                if (!hasRolled) {
                    displayPlayerProperties(game, board, game->currentPlayer);
                    cout << "Enter property number to mortgage (0 to cancel): ";
                    int propNum;
                    cin >> propNum;
                    if (propNum > 0) {
                        mortgageProperty(game, board, currentPlayer->ownedProperties[propNum - 1]);
                    }
                } else {
                    cout << "Cannot mortgage properties after rolling dice!\n";
                }
                break;
            }
            
            case 7: { // Unmortgage Property
                if (!hasRolled) {
                    displayPlayerProperties(game, board, game->currentPlayer);
                    cout << "Enter property number to unmortgage (0 to cancel): ";
                    int propNum;
                    cin >> propNum;
                    if (propNum > 0) {
                        unmortgageProperty(game, board, currentPlayer->ownedProperties[propNum - 1]);
                    }
                } else {
                    cout << "Cannot unmortgage properties after rolling dice!\n";
                }
                break;
            }
            
            case 8: // Save Game
                saveGame(game, board);
                break;
                
            case 9: // Quit
                game->gameOver = true;
                turnEnded = true;
                break;
                
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
        
        if (currentPlayer->money < 0) {
            cout << "\nWARNING: You are in debt! Amount: $" << currentPlayer->money << endl;
            handleBankruptcy(game, board);
            turnEnded = true;
        }
    }
    
    game->currentPlayer = (game->currentPlayer + 1) % game->numPlayers;
}

