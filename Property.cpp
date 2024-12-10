#include "Property.h"
#include <iostream>

// StandardProperty implementations
StandardProperty::StandardProperty(const std::string& n, int pos, int p, int rent, std::string col)
    : Property(n, pos, p), baseRent(rent), houses(0), hotels(0), color(col) {}

int StandardProperty::calculateRent() const {
    if (mortgaged) return 0;
    
    int rent = baseRent;
    if (hotels > 0) {
        rent *= 5;
    } else if (houses > 0) {
        rent *= (1 + houses);
    }
    return rent;
}

void StandardProperty::develop() {
    if (hotels == MAX_HOTELS) {
        throw GameException("Maximum development level reached");
    }
    
    if (houses == MAX_HOUSES) {
        houses = 0;
        hotels++;
    } else {
        houses++;
    }
}

int StandardProperty::getCurrentDevelopmentLevel() const {
    return houses + (hotels * 5);
}

int StandardProperty::getDevelopmentCost() const {
    return (hotels == 0) ? HOUSE_COST : HOTEL_COST;
}

// RailroadProperty implementations
RailroadProperty::RailroadProperty(const std::string& n, int pos)
    : Property(n, pos, 200) {} // Railroads cost $200

int RailroadProperty::calculateRent() const {
    if (mortgaged) return 0;
    return 25; // Base rent for one railroad
}

void RailroadProperty::develop() {
    throw GameException("Railroad properties cannot be developed.");
}

// UtilityProperty implementations
UtilityProperty::UtilityProperty(const std::string& n, int pos)
    : Property(n, pos, 150) {} // Utilities cost $150

int UtilityProperty::calculateRent() const {
    if (mortgaged) return 0;
    return 10; // Base utility rent
}

void UtilityProperty::develop() {
    throw GameException("Utility properties cannot be developed.");
}

// Stream operator implementation
std::ostream& operator<<(std::ostream& os, const Property& prop) {
    os << prop.getName() << " (" << prop.getType() << ")"
       << " - Price: $" << prop.getPrice()
       << " - Owner: " << prop.getOwner()
       << (prop.isMortgaged() ? " (Mortgaged)" : "");

    if (auto stdProp = dynamic_cast<const StandardProperty*>(&prop)) {
        os << " - Color: " << stdProp->getColor();
    }
    return os;
}