#include "Board.h"
#include <iostream>
#include "Player.h"
#include "Property.h"
#include "Bank.h"
#include <stdexcept>
#include <string>
#include <memory>
#include <random>
#include <algorithm>
#include <functional>  

class InsufficientFundsException : public std::exception {
   const char* what() const noexcept override { return "Insufficient funds"; }
};

PropertySpace::PropertySpace(int pos, std::shared_ptr<Property> prop)
   : Space(pos, SpaceType::PROPERTY, prop->getName()), property(prop) {}

void PropertySpace::landOn(Player& player, Bank& bank) {
    if (property->getOwner() == "Bank") {
        std::cout << "\nWould you like to purchase " << property->getName() 
                  << " for $" << property->getPrice() << "? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore(100, '\n');  // Clear input buffer, using simpler version for NetBeans

        if (choice == 'y' || choice == 'Y') {
            if (!player.canAfford(property->getPrice())) {
                std::cout << "Sorry, you don't have enough money to purchase this property.";
                std::cout << "\nYour balance: $" << player.getMoney() << std::endl;           
                return;
            }
            if (bank.sellProperty(player, property)) {
                std::cout << "Congratulations! You now own " << property->getName() << "!";
                std::cout << "\nYour new balance: $" << player.getMoney() << std::endl;
            }
        } else {
            std::cout << "Property purchase declined.";
            std::cout << "\nYour balance: $" << player.getMoney() << std::endl;
            return;
        }
    } else if (property->getOwner() != player.getName() && !property->isMortgaged()) {
        int rent = property->calculateRent();
        std::cout << "\nYou must pay $" << rent << " rent to " << property->getOwner() << ".";
        try {
            player -= rent;
            std::cout << "\nRent paid. Your new balance: $" << player.getMoney() << std::endl;
        } catch (const InsufficientFundsException&) {
            std::cout << "\nYou don't have enough money to pay the rent!";
            std::cout << "\nYour balance: $" << player.getMoney() << std::endl;
            bank.handleBankruptcy(player);
        }
    }
}

TaxSpace::TaxSpace(int pos, const std::string& n, int amount)
   : Space(pos, SpaceType::TAX, n), taxAmount(amount) {}

void TaxSpace::landOn(Player& player, Bank& bank) {
   std::cout << "\nYou must pay $" << taxAmount << " in " << getName() << ".";
   try {
       player -= taxAmount;
       std::cout << "\nTax paid. Your new balance: $" << player.getMoney() << std::endl;
   } catch (const InsufficientFundsException&) {
       bank.handleBankruptcy(player);
   }
}

ChanceSpace::ChanceSpace(int pos) : Space(pos, SpaceType::CHANCE, "Chance") {}

void ChanceSpace::landOn(Player& player, Bank& bank) {
    std::cout << "\nDrawing Chance card...\n";
}

CommunityChestSpace::CommunityChestSpace(int pos)
   : Space(pos, SpaceType::COMMUNITY_CHEST, "Community Chest") {}

void CommunityChestSpace::landOn(Player& player, Bank& bank) {
    std::cout << "\nDrawing Community Chest card...\n";
}

JailSpace::JailSpace(int pos) : Space(pos, SpaceType::JAIL, "Jail") {}

void JailSpace::landOn(Player& player, Bank& bank) {}

GoSpace::GoSpace() : Space(0, SpaceType::GO, "GO") {}

void GoSpace::landOn(Player& player, Bank& bank) {
   player += PASS_GO_AMOUNT;
   std::cout << "\nYou landed on GO! Collect $" << PASS_GO_AMOUNT << "!"
             << "\nYour new balance: $" << player.getMoney() << std::endl;
}

FreeParkingSpace::FreeParkingSpace() 
    : Space(20, SpaceType::FREE_PARKING, "Free Parking"), pot(0) {}

void FreeParkingSpace::landOn(Player& player, Bank& bank) {
   if (pot > 0) {
       std::cout << "\nYou collected $" << pot << " from Free Parking!" << std::endl;
       player += pot;
       std::cout << "Your new balance: $" << player.getMoney() << std::endl;
       pot = 0;
   } else {
       std::cout << "\nFree Parking pot is empty." << std::endl;
   }
}

GoToJailSpace::GoToJailSpace()
   : Space(30, SpaceType::GO_TO_JAIL, "Go To Jail") {}

void GoToJailSpace::landOn(Player& player, Bank& bank) {
   player.goToJail();
}

Board::Board(Bank& b) : bank(b) {
   initializeSpaces();
   initializeCards();
   shuffleCards();
}

void Board::initializeSpaces() {
    // Initialize all spaces with empty unique pointers first
    for (auto& space : spaces) {
        space = nullptr;
    }

    // Special spaces
    spaces[0] = std::make_unique<GoSpace>();
    spaces[10] = std::make_unique<JailSpace>(10);
    spaces[20] = std::make_unique<FreeParkingSpace>();
    spaces[30] = std::make_unique<GoToJailSpace>();

    // Brown properties
    spaces[1] = std::make_unique<PropertySpace>(1, 
        std::make_shared<StandardProperty>("Mediterranean Avenue", 1, 60, 2, "Brown"));
    spaces[3] = std::make_unique<PropertySpace>(3,
        std::make_shared<StandardProperty>("Baltic Avenue", 3, 60, 4, "Brown"));

    // Light Blue properties
    spaces[6] = std::make_unique<PropertySpace>(6,
        std::make_shared<StandardProperty>("Oriental Avenue", 6, 100, 6, "Light Blue"));
    spaces[8] = std::make_unique<PropertySpace>(8,
        std::make_shared<StandardProperty>("Vermont Avenue", 8, 100, 6, "Light Blue"));
    spaces[9] = std::make_unique<PropertySpace>(9,
        std::make_shared<StandardProperty>("Connecticut Avenue", 9, 120, 8, "Light Blue"));

    // Pink properties
    spaces[11] = std::make_unique<PropertySpace>(11,
        std::make_shared<StandardProperty>("St. Charles Place", 11, 140, 10, "Pink"));
    spaces[13] = std::make_unique<PropertySpace>(13,
        std::make_shared<StandardProperty>("States Avenue", 13, 140, 10, "Pink"));
    spaces[14] = std::make_unique<PropertySpace>(14,
        std::make_shared<StandardProperty>("Virginia Avenue", 14, 160, 12, "Pink"));

    // Orange properties
    spaces[16] = std::make_unique<PropertySpace>(16,
        std::make_shared<StandardProperty>("St. James Place", 16, 180, 14, "Orange"));
    spaces[18] = std::make_unique<PropertySpace>(18,
        std::make_shared<StandardProperty>("Tennessee Avenue", 18, 180, 14, "Orange"));
    spaces[19] = std::make_unique<PropertySpace>(19,
        std::make_shared<StandardProperty>("New York Avenue", 19, 200, 16, "Orange"));

    // Red properties
    spaces[21] = std::make_unique<PropertySpace>(21,
        std::make_shared<StandardProperty>("Kentucky Avenue", 21, 220, 18, "Red"));
    spaces[23] = std::make_unique<PropertySpace>(23,
        std::make_shared<StandardProperty>("Indiana Avenue", 23, 220, 18, "Red"));
    spaces[24] = std::make_unique<PropertySpace>(24,
        std::make_shared<StandardProperty>("Illinois Avenue", 24, 240, 20, "Red"));

    // Yellow properties
    spaces[26] = std::make_unique<PropertySpace>(26,
        std::make_shared<StandardProperty>("Atlantic Avenue", 26, 260, 22, "Yellow"));
    spaces[27] = std::make_unique<PropertySpace>(27,
        std::make_shared<StandardProperty>("Ventnor Avenue", 27, 260, 22, "Yellow"));
    spaces[29] = std::make_unique<PropertySpace>(29,
        std::make_shared<StandardProperty>("Marvin Gardens", 29, 280, 24, "Yellow"));

    // Green properties
    spaces[31] = std::make_unique<PropertySpace>(31,
        std::make_shared<StandardProperty>("Pacific Avenue", 31, 300, 26, "Green"));
    spaces[32] = std::make_unique<PropertySpace>(32,
        std::make_shared<StandardProperty>("North Carolina Avenue", 32, 300, 26, "Green"));
    spaces[34] = std::make_unique<PropertySpace>(34,
        std::make_shared<StandardProperty>("Pennsylvania Avenue", 34, 320, 28, "Green"));

    // Dark Blue properties
    spaces[37] = std::make_unique<PropertySpace>(37,
        std::make_shared<StandardProperty>("Park Place", 37, 350, 35, "Dark Blue"));
    spaces[39] = std::make_unique<PropertySpace>(39,
        std::make_shared<StandardProperty>("Boardwalk", 39, 400, 50, "Dark Blue"));

    // Railroad properties
    spaces[5] = std::make_unique<PropertySpace>(5,
        std::make_shared<RailroadProperty>("Reading Railroad", 5));
    spaces[15] = std::make_unique<PropertySpace>(15,
        std::make_shared<RailroadProperty>("Pennsylvania Railroad", 15));
    spaces[25] = std::make_unique<PropertySpace>(25,
        std::make_shared<RailroadProperty>("B. & O. Railroad", 25));
    spaces[35] = std::make_unique<PropertySpace>(35,
        std::make_shared<RailroadProperty>("Short Line", 35));

    // Utility properties
    spaces[12] = std::make_unique<PropertySpace>(12,
        std::make_shared<UtilityProperty>("Electric Company", 12));
    spaces[28] = std::make_unique<PropertySpace>(28,
        std::make_shared<UtilityProperty>("Water Works", 28));

    // Tax spaces
    spaces[4] = std::make_unique<TaxSpace>(4, "Income Tax", 200);
    spaces[38] = std::make_unique<TaxSpace>(38, "Luxury Tax", 100);

    // Community Chest spaces
    spaces[2] = std::make_unique<CommunityChestSpace>(2);
    spaces[17] = std::make_unique<CommunityChestSpace>(17);
    spaces[33] = std::make_unique<CommunityChestSpace>(33);

    // Chance spaces
    spaces[7] = std::make_unique<ChanceSpace>(7);
    spaces[22] = std::make_unique<ChanceSpace>(22);
    spaces[36] = std::make_unique<ChanceSpace>(36);
}

void Board::initializeCards() {
    // Chance Cards
    addChanceCard("Advance to GO", [this](Player& p) { 
        teleportPlayer(p, 0);  // Already on GO, no need to check passing
    });
    
    addChanceCard("Go to Jail", [](Player& p) { 
        p.goToJail();  // No money for passing GO when going to jail
    });
    
    addChanceCard("Advance to Illinois Avenue", [this](Player& p) { 
        teleportPlayer(p, 24);
    });
    
    addChanceCard("Advance to St. Charles Place", [this](Player& p) { 
        teleportPlayer(p, 11);
    });
    
    addChanceCard("Take a trip to Reading Railroad", [this](Player& p) { 
        teleportPlayer(p, 5);
    });
    
    addChanceCard("Bank pays you dividend of $50", [](Player& p) { 
        p += 50;
    });
    
    // Community Chest Cards 
    addCommunityCard("Bank error in your favor - Collect $200", [](Player& p) { p += 200; });
    addCommunityCard("Doctor's fee - Pay $50", [](Player& p) { p -= 50; });
    addCommunityCard("From sale of stock you get $45", [](Player& p) { p += 45; });
    addCommunityCard("Pay hospital fees of $100", [](Player& p) { p -= 100; });
    addCommunityCard("Life insurance matures – Collect $100", [](Player& p) { p += 100; });
    addCommunityCard("Pay school fees of $150", [](Player& p) { p -= 150; });
}

void Board::shuffleCards() {
   std::random_device rd;
   std::mt19937 g(rd());
   std::shuffle(chanceCards.begin(), chanceCards.end(), g);
   std::shuffle(communityCards.begin(), communityCards.end(), g);
}

void Board::movePlayer(Player& player, int spaces) {
    int newPosition = (player.getPosition() + spaces) % BOARD_SIZE;
    if (newPosition < player.getPosition()) {
        handlePassGo(player);
    }
    player.moveToPosition(newPosition);
    
    std::unique_ptr<Space>& space = this->spaces[newPosition];
    if (!space) {
        throw std::runtime_error("Attempting to access uninitialized board space");
    }
    
    std::cout << "\nLanded on: " << space->getName();
    if (auto propertySpace = dynamic_cast<PropertySpace*>(space.get())) {
        auto property = propertySpace->getProperty();
        std::cout << " - Price: $" << property->getPrice();
        
        if (auto stdProp = std::dynamic_pointer_cast<StandardProperty>(property)) {
            std::cout << " - Color: " << stdProp->getColor();
        }
        
        if (property->getOwner() != "Bank") {
            std::cout << " (Owned by: " << property->getOwner() << ")";
        }
    }
    std::cout << std::endl;
    
    // Execute the space's landOn action
    space->landOn(player, bank);
    
    // Handle card spaces
    if (space->getType() == SpaceType::CHANCE) {
        std::cout << "\nDrawing a chance card..." << std::endl;
        drawChanceCard(player);
    } else if (space->getType() == SpaceType::COMMUNITY_CHEST) {
        std::cout << "\nDrawing a community chest card..." << std::endl;
        drawCommunityCard(player);
    }
}

void Board::teleportPlayer(Player& player, int position) {
   int oldPosition = player.getPosition();
   player.moveToPosition(position);
   if (position < oldPosition && position != 10) {
       handlePassGo(player);
   }
   spaces[position]->landOn(player, bank);
}

void Board::handlePassGo(Player& player) {
   player += 200;
   std::cout << "You passed GO! Collect $200!\n";
   std::cout << "Your new balance: $" << player.getMoney() << std::endl;
}

void Board::drawChanceCard(Player& player) {
    std::cout << "Processing chance card..." << std::endl;
    if (chanceCards.empty()) {
        shuffleCards();
    }
    Card card = chanceCards.back();
    chanceCards.pop_back();
    
    std::cout << "Card says: " << card.getDescription() << std::endl;
    
    card.execute(player);
    std::cout << "Your new balance: $" << player.getMoney() << std::endl;
    chanceCards.insert(chanceCards.begin(), card);
}


void Board::drawCommunityCard(Player& player) {
    std::cout << "Processing community chest card..." << std::endl;
    if (communityCards.empty()) {
        shuffleCards();
    }
    Card card = communityCards.back();
    communityCards.pop_back();
    
    std::cout << "Card says: " << card.getDescription() << std::endl;
    
    try {
        card.execute(player);
        std::cout << "Your new balance: $" << player.getMoney() << std::endl;
    } catch (const InsufficientFundsException&) {
        bank.handleBankruptcy(player);
    }
    communityCards.insert(communityCards.begin(), card);
}

Space* Board::getSpace(int position) const {
   return spaces[position].get();
}

void Board::addChanceCard(const std::string& description, std::function<void(Player&)> action) {
   chanceCards.emplace_back(description, action);
}

void Board::addCommunityCard(const std::string& description, std::function<void(Player&)> action) {
   communityCards.emplace_back(description, action);
}