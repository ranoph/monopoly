#include "Bank.h"
#include "Player.h"
#include "Board.h"
#include <stdexcept>

Bank::Bank() 
    : houseManager(INITIAL_HOUSES, INITIAL_HOUSES, "Houses"),
      hotelManager(INITIAL_HOTELS, INITIAL_HOTELS, "Hotels"),
      moneyManager(INITIAL_MONEY, INITIAL_MONEY, "Money"),
      board(nullptr) {
    initializePropertyValues();
}

void Bank::initializePropertyValues() {
   propertyValues = {
       {"Mediterranean Avenue", 60},
       {"Baltic Avenue", 60},
       {"Reading Railroad", 200},
       {"Oriental Avenue", 100},
       {"Vermont Avenue", 100},
       {"Connecticut Avenue", 120}
   };
}

bool Bank::sellProperty(Player& buyer, std::shared_ptr<Property> property) {
   if (!validateTransaction(buyer, property->getPrice())) {
       return false;
   }
   buyer -= property->getPrice();
   buyer.addProperty(property);
   return true;
}

bool Bank::auctionProperty(std::vector<std::reference_wrapper<Player>>& players, std::shared_ptr<Property> property) {
   if (players.empty()) return false;
   
   int currentBid = 0;
   int winningBid = 0;
   Player* winner = nullptr;
   bool hasBids;
   
   do {
       hasBids = false;
       for (auto& playerRef : players) {
           Player& player = playerRef.get();
           if (player.canAfford(currentBid + 10)) {
               currentBid += 10;
               winningBid = currentBid;
               winner = &player;
               hasBids = true;
           }
       }
   } while (hasBids);
   
   if (winner && winningBid > 0) {
       *winner -= winningBid;
       winner->addProperty(property);
       return true;
   }
   return false;
}

void Bank::foreclosureProperty(Player& player, std::shared_ptr<Property> property) {
   player.removeProperty(property);
   foreclosedProperties.push(property);
   
   auto stdProp = std::dynamic_pointer_cast<StandardProperty>(property);
   if (stdProp) {
       while (stdProp->getCurrentDevelopmentLevel() > 0) {
           if (stdProp->getHotels() > 0) {
               returnHotel(stdProp);
           } else {
               returnHouse(stdProp);
           }
       }
   }
}

bool Bank::buyHouse(Player& player, std::shared_ptr<StandardProperty> property) {
    if (houseManager.isEmpty() || !canDevelop(player, property)) {
        return false;
    }
    
    int cost = property->getDevelopmentCost();
    if (!validateTransaction(player, cost)) {
        return false;
    }
    
    if (!houseManager.consume(1)) {
        return false;
    }
    
    player -= cost;
    property->develop();
    developedProperties[property->getColor()].insert(property);
    return true;
}

bool Bank::buyHotel(Player& player, std::shared_ptr<StandardProperty> property) {
    if (hotelManager.isEmpty() || !canDevelop(player, property)) {
        return false;
    }
    
    int cost = property->getDevelopmentCost();
    if (!validateTransaction(player, cost)) {
        return false;
    }
    
    if (!hotelManager.consume(1)) {
        return false;
    }
    
    player -= cost;
    property->develop();
    houseManager.add(4);  // Return 4 houses when buying a hotel
    return true;
}

void Bank::returnHouse(std::shared_ptr<StandardProperty> property) {
    if (property->getHouses() > 0) {
        property->develop();  // Use develop() to handle house removal
        houseManager.add(1);
        if (property->getHouses() == 0) {
            developedProperties[property->getColor()].erase(property);
        }
    }
}

void Bank::returnHotel(std::shared_ptr<StandardProperty> property) {
    if (property->getHotels() > 0) {
        property->develop();  // Use develop() to handle hotel removal
        hotelManager.add(1);
        if (!houseManager.consume(4)) {  // Take back 4 houses when removing a hotel
            // Handle error if needed
        }
    }
}
bool Bank::validateTransaction(const Player& player, int amount) const {
    return player.canAfford(amount);
}

bool Bank::canDevelop(const Player& player, std::shared_ptr<StandardProperty> property) const {
    if (!player.hasMonopoly(property->getColor())) {
        return false;
    }
    
    auto colorProps = player.getProperties();
    int currentLevel = property->getCurrentDevelopmentLevel();
    
    for (const auto& prop : colorProps) {
        auto stdProp = std::dynamic_pointer_cast<StandardProperty>(prop);
        if (stdProp && stdProp->getColor() == property->getColor() && 
            stdProp->getCurrentDevelopmentLevel() < currentLevel) {
            return false;
        }
    }
    
    return true;
}

void Bank::handleBankruptcy(Player& bankruptPlayer, Player* creditorPlayer) {
    for (auto& property : bankruptPlayer.getProperties()) {
        if (creditorPlayer) {
            bankruptPlayer.removeProperty(property);
            creditorPlayer->addProperty(property);
        } else {
            foreclosureProperty(bankruptPlayer, property);
        }
    }
    bankruptPlayer.money = 0;
}

bool Bank::mortgageProperty(Player& player, std::shared_ptr<Property> property) {
    if (!player.ownsProperty(property) || property->isMortgaged()) {
        return false;
    }
    
    int mortgageValue = property->getMortgageValue();
    if (!moneyManager.consume(mortgageValue)) {
        return false;
    }
    
    property->setMortgaged(true);
    player += mortgageValue;
    return true;
}

bool Bank::unmortgageProperty(Player& player, std::shared_ptr<Property> property) {
    if (!player.ownsProperty(property) || !property->isMortgaged()) {
        return false;
    }
    
    int unmortgageCost = static_cast<int>(property->getMortgageValue() * 1.1);
    if (!validateTransaction(player, unmortgageCost)) {
        return false;
    }
    
    if (!moneyManager.add(unmortgageCost)) {
        return false;
    }
    
    player -= unmortgageCost;
    property->setMortgaged(false);
    return true;
}