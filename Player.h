#ifndef PLAYER_H
#define PLAYER_H
#include "Property.h"
#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <functional>
#include <ostream>

class Card;

/**
 * @brief Represents a player in the Monopoly game
 */
class Player {
private:
    std::string name;                                     ///< Player name
    int money;                                           ///< Current money
    int position;                                        ///< Board position
    std::vector<std::shared_ptr<Property>> properties;   ///< Owned properties
    bool inJail;                                         ///< Jail status
    int jailTurns;                                       ///< Turns spent in jail
    int jailRollAttempts = 0;                           ///< Attempts to roll doubles
    std::deque<Card> cards;                             ///< Held cards
    static const int MAX_JAIL_TURNS = 3;                ///< Max turns in jail
    static const int JAIL_FINE = 50;                    ///< Jail release cost

public:
    /**
     * @brief Constructor for player
     * @param n Player name
     * @param startingMoney Initial money amount
     */
    Player(const std::string& n, int startingMoney = 1500);
    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;
    
    /**
     * @brief Check if player can afford amount
     * @param amount Money amount
     * @return true if player has sufficient funds
     */
    bool canAfford(int amount) const { return money >= amount; }

    /**
     * @brief Add money operator
     * @param amount Amount to add
     * @return Player reference
     */
    Player& operator+=(int amount);

    /**
     * @brief Subtract money operator
     * @param amount Amount to subtract
     * @return Player reference
     * @throws InsufficientFundsException
     */
    Player& operator-=(int amount);
    
    // Property operations
    void addProperty(std::shared_ptr<Property> prop);              ///< Add property
    void removeProperty(std::shared_ptr<Property> prop);           ///< Remove property
    bool ownsProperty(const std::shared_ptr<Property>& prop) const; ///< Check ownership
    bool hasMonopoly(const std::string& color) const;              ///< Check monopoly
    const std::vector<std::shared_ptr<Property>>& getProperties() const { return properties; }
    std::vector<std::shared_ptr<Property>> getPropertiesOfColor(const std::string& color) const;
    bool canDevelopProperty(const std::shared_ptr<Property>& prop) const;
    void developProperty(std::shared_ptr<Property> prop);
    void mortgageProperty(std::shared_ptr<Property> prop);
    void unmortgageProperty(std::shared_ptr<Property> prop);
    
    // Game state getters/setters
    const std::string& getName() const { return name; }
    int getMoney() const { return money; }
    int getPosition() const { return position; }
    bool isInJail() const { return inJail; }
    void goToJail();                                    ///< Send to jail
    void setInJail(bool status) { inJail = status; }
    bool tryToLeaveJail();                             ///< Attempt jail exit
    void payToLeaveJail();                             ///< Pay jail fine
    void moveToPosition(int pos);                       ///< Move to position
    void movePlayer(int spaces);                        ///< Move spaces forward
    
    // Card operations
    void addCard(const Card& card);                     ///< Add card to hand
    void useCard(size_t index);                         ///< Use card at index
    
    // Player interactions
    void transferMoneyTo(Player& other, int amount);    ///< Transfer money
    void transferPropertyTo(Player& other, std::shared_ptr<Property> prop); ///< Transfer property
    
    friend std::ostream& operator<<(std::ostream& os, const Player& player);
    friend class Bank;
    friend class Board;
};
#endif