#ifndef BOARD_H
#define BOARD_H
#include <array>
#include <memory>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include "Bank.h"
#include "Card.h"
#include "Player.h"
#include "Property.h"

/**
 * @brief Defines the types of spaces on the Monopoly board
 */
enum class SpaceType {
    PROPERTY,
    TAX,
    CHANCE,
    COMMUNITY_CHEST,
    JAIL,
    GO,
    FREE_PARKING,
    GO_TO_JAIL
};

/**
 * @brief Base class for board spaces
 */
class Space {
protected:
    int position;      ///< Space position on board
    SpaceType type;    ///< Type of space
    std::string name;  ///< Name of space
public:
    /**
     * @brief Construct a new Space object
     * @param pos Position on board
     * @param t Type of space
     * @param n Name of space
     */
    Space(int pos, SpaceType t, const std::string& n) : 
        position(pos), type(t), name(n) {}
    virtual ~Space() = default;
    
    /**
     * @brief Action when player lands on space
     * @param player Player landing on space
     * @param bank Bank instance for transactions
     */
    virtual void landOn(Player& player, Bank& bank) = 0;
    SpaceType getType() const { return type; }
    std::string getName() const { return name; }
    int getPosition() const { return position; }
};

/**
 * @brief Property space that can be owned and developed
 */
class PropertySpace : public Space {
private:
    std::shared_ptr<Property> property;  ///< Property on this space
public:
    PropertySpace(int pos, std::shared_ptr<Property> prop);
    void landOn(Player& player, Bank& bank) override;
    std::shared_ptr<Property> getProperty() const { return property; }
};

/**
 * @brief Tax space requiring payment
 */
class TaxSpace : public Space {
private:
    int taxAmount;  ///< Amount of tax to pay
public:
    TaxSpace(int pos, const std::string& n, int amount);
    void landOn(Player& player, Bank& bank) override;
};

/**
 * @brief Chance card space
 */
class ChanceSpace : public Space {
public:
    ChanceSpace(int pos);
    void landOn(Player& player, Bank& bank) override;
};

/**
 * @brief Community chest card space
 */
class CommunityChestSpace : public Space {
public:
    CommunityChestSpace(int pos);
    void landOn(Player& player, Bank& bank) override;
};

/**
 * @brief Jail/Just Visiting space
 */
class JailSpace : public Space {
public:
    JailSpace(int pos);
    void landOn(Player& player, Bank& bank) override;
};

/**
 * @brief GO space providing money when passed
 */
class GoSpace : public Space {
private:
    static const int PASS_GO_AMOUNT = 200;  ///< Amount collected for passing GO
public:
    GoSpace();
    void landOn(Player& player, Bank& bank) override;
};

/**
 * @brief Free Parking space that collects fines
 */
class FreeParkingSpace : public Space {
private:
    int pot;  ///< Accumulated money from fines
public:
    FreeParkingSpace(); 
    virtual void landOn(Player& player, Bank& bank) override;
    void addToPot(int amount) { pot += amount; }
};

/**
 * @brief Space that sends player to jail
 */
class GoToJailSpace : public Space {
public:
    GoToJailSpace();
    void landOn(Player& player, Bank& bank) override;
};

/**
 * @brief Main board class managing game state and movement
 */
class Board {
private:
    static const int BOARD_SIZE = 40;
    std::array<std::unique_ptr<Space>, BOARD_SIZE> spaces;
    Bank& bank;
    std::vector<Card> chanceCards;
    std::vector<Card> communityCards;
    
    void shuffleCards();
    void initializeSpaces();
    void initializeCards();
public:
    explicit Board(Bank& b);
    void movePlayer(Player& player, int spaces);
    void teleportPlayer(Player& player, int position);
    Space* getSpace(int position) const;
    void drawChanceCard(Player& player);
    void drawCommunityCard(Player& player);
    
    void addChanceCard(const std::string& description, std::function<void(Player&)> action);
    void addCommunityCard(const std::string& description, std::function<void(Player&)> action);
    
private:
    void handlePassGo(Player& player);
    void createPropertyCards();
};
#endif