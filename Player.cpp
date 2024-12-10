#include "Player.h"
#include "Card.h"
#include "GameExceptions.h"
#include <algorithm>

Player::Player(const std::string& n, int startingMoney) 
    : name(n), money(startingMoney), position(0), inJail(false), jailTurns(0) {}

Player& Player::operator+=(int amount) {
    money += amount;
    return *this;
}

Player& Player::operator-=(int amount) {
    if (money - amount < 0)
        throw InsufficientFundsException("Not enough money for transaction");
    money -= amount;
    return *this;
}

void Player::addProperty(std::shared_ptr<Property> prop) {
    properties.push_back(prop);
    prop->setOwner(name);
}

void Player::removeProperty(std::shared_ptr<Property> prop) {
    auto it = std::find(properties.begin(), properties.end(), prop);
    if (it != properties.end()) {
        properties.erase(it);
        prop->setOwner("Bank");
    }
}

bool Player::ownsProperty(const std::shared_ptr<Property>& prop) const {
    return std::find(properties.begin(), properties.end(), prop) != properties.end();
}

bool Player::hasMonopoly(const std::string& color) const {
    int colorCount = 0;
    int totalOfColor = 0;
    
    for (const auto& prop : properties) {
        auto stdProp = std::dynamic_pointer_cast<StandardProperty>(prop);
        if (stdProp && stdProp->getColor() == color) {
            colorCount++;
        }
    }
    
    if (color == "Brown" || color == "Dark Blue") {
        totalOfColor = 2;
    } else {
        totalOfColor = 3;
    }
    
    return colorCount == totalOfColor;
}

std::vector<std::shared_ptr<Property>> Player::getPropertiesOfColor(const std::string& color) const {
    std::vector<std::shared_ptr<Property>> colorProps;
    for (const auto& prop : properties) {
        auto stdProp = std::dynamic_pointer_cast<StandardProperty>(prop);
        if (stdProp && stdProp->getColor() == color) {
            colorProps.push_back(prop);
        }
    }
    return colorProps;
}

void Player::movePlayer(int spaces) {
    position = (position + spaces) % 40;
}

void Player::moveToPosition(int newPosition) {
    position = newPosition % 40;
}

void Player::goToJail() {
    inJail = true;
    position = 10;
    jailTurns = 0;
}

bool Player::tryToLeaveJail() {
    jailTurns++;
    if (jailTurns >= MAX_JAIL_TURNS) {
        inJail = false;
        jailTurns = 0;
        return true;
    }
    return false;
}

void Player::payToLeaveJail() {
    *this -= JAIL_FINE;
    inJail = false;
    jailTurns = 0;
}

void Player::addCard(const Card& card) {
    cards.push_back(card);
}

void Player::useCard(size_t index) {
    if (index >= cards.size()) {
        throw GameException("Invalid card index");
    }
    cards[index].execute(*this);
    if (!cards[index].isKeepable()) {
        cards.erase(cards.begin() + index);
    }
}

void Player::transferMoneyTo(Player& other, int amount) {
    if (!canAfford(amount)) {
        throw InsufficientFundsException("Insufficient funds for transfer");
    }
    *this -= amount;
    other += amount;
}

void Player::transferPropertyTo(Player& other, std::shared_ptr<Property> prop) {
    if (!ownsProperty(prop)) {
        throw GameException("Player doesn't own this property");
    }
    removeProperty(prop);
    other.addProperty(prop);
}

bool Player::canDevelopProperty(const std::shared_ptr<Property>& prop) const {
    auto stdProp = std::dynamic_pointer_cast<StandardProperty>(prop);
    return stdProp && ownsProperty(prop) && hasMonopoly(stdProp->getColor());
}

void Player::developProperty(std::shared_ptr<Property> prop) {
    if (!canDevelopProperty(prop)) {
        throw GameException("Cannot develop this property");
    }
    auto stdProp = std::dynamic_pointer_cast<StandardProperty>(prop);
    stdProp->develop();
}

void Player::mortgageProperty(std::shared_ptr<Property> prop) {
    if (!ownsProperty(prop) || prop->isMortgaged()) {
        throw GameException("Cannot mortgage this property");
    }
    prop->setMortgaged(true);
    *this += prop->getMortgageValue();
}

void Player::unmortgageProperty(std::shared_ptr<Property> prop) {
    if (!ownsProperty(prop) || !prop->isMortgaged()) {
        throw GameException("Cannot unmortgage this property");
    }
    int cost = static_cast<int>(prop->getMortgageValue() * 1.1);
    if (!canAfford(cost)) {
        throw InsufficientFundsException("Cannot afford to unmortgage");
    }
    *this -= cost;
    prop->setMortgaged(false);
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << player.getName() << " ($" << player.getMoney() << ")";
    return os;
}