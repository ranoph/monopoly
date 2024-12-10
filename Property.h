#ifndef PROPERTY_H
#define PROPERTY_H
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

/**
 * @brief Custom exception for game errors
 */
class GameException : public std::exception {
private:
    std::string message;
public:
    explicit GameException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

/**
 * @brief Base class for all properties on the board
 */
class Property {
protected:
    std::string name;    ///< Property name
    int position;        ///< Board position
    int price;          ///< Purchase price
    std::string owner;   ///< Current owner
    bool mortgaged;      ///< Mortgage status
    
public:
    Property(const std::string& n, int pos, int p) : 
        name(n), position(pos), price(p), owner("Bank"), mortgaged(false) {}
    virtual ~Property() = default;
    
    virtual int calculateRent() const = 0;
    virtual std::string getType() const = 0;
    virtual bool canBeDeveloped() const = 0;
    virtual int getDevelopmentCost() const = 0;
    virtual int getCurrentDevelopmentLevel() const = 0;
    virtual void develop() = 0;
    virtual int getMortgageValue() const { return price / 2; }
    
    virtual std::string getName() const { return name; }
    virtual int getPosition() const { return position; }
    virtual int getPrice() const { return price; }
    virtual std::string getOwner() const { return owner; }
    virtual void setOwner(const std::string& newOwner) { owner = newOwner; }
    virtual bool isMortgaged() const { return mortgaged; }
    virtual void setMortgaged(bool status) { mortgaged = status; }
};

/**
 * @brief Property that can be developed with houses and hotels
 */
class StandardProperty : public Property {
private:
    int baseRent;         ///< Base rent amount
    int houses;           ///< Number of houses
    int hotels;           ///< Number of hotels
    std::string color;    ///< Property color group
    static const int MAX_HOUSES = 4;
    static const int MAX_HOTELS = 1;
    static const int HOUSE_COST = 100;
    static const int HOTEL_COST = 100;
    
public:
    StandardProperty(const std::string& n, int pos, int p, int rent, std::string col);
    int calculateRent() const override;
    std::string getType() const override { return "Standard"; }
    bool canBeDeveloped() const override { return true; }
    int getDevelopmentCost() const override;
    int getCurrentDevelopmentLevel() const override;
    void develop() override;
    std::string getColor() const { return color; }
    int getHouses() const { return houses; }
    int getHotels() const { return hotels; }
};

/**
 * @brief Railroad property with fixed rent scaling
 */
class RailroadProperty : public Property {
private:
    static const int BASE_RENT = 25;
    static const std::vector<int> RENT_MULTIPLIERS;
    
public:
    RailroadProperty(const std::string& n, int pos);
    int calculateRent() const override;
    std::string getType() const override { return "Railroad"; }
    bool canBeDeveloped() const override { return false; }
    int getDevelopmentCost() const override { return 0; }
    int getCurrentDevelopmentLevel() const override { return 0; }
    void develop() override;
};

/**
 * @brief Utility property with dice-based rent
 */
class UtilityProperty : public Property {
private:
    static const int MULTIPLIER_ONE = 4;
    static const int MULTIPLIER_BOTH = 10;
    
public:
    UtilityProperty(const std::string& n, int pos);
    int calculateRent() const override;
    std::string getType() const override { return "Utility"; }
    bool canBeDeveloped() const override { return false; }
    int getDevelopmentCost() const override { return 0; }
    int getCurrentDevelopmentLevel() const override { return 0; }
    void develop() override;
};

std::ostream& operator<<(std::ostream& os, const Property& prop);
#endif