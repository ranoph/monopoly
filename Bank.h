#ifndef BANK_H
#define BANK_H
#include "ResourceManager.h"
#include "GameState.h"
#include "Property.h"
#include "Player.h"
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <memory>

class Board;  // Forward declaration

/**
 * @brief Manages the game's banking system and resources
 * @details Handles property transactions, development, and resource management
 */
class Bank {
private:
    /**
     * @brief Internal state of bank resources
     */
    struct BankState {
        int houses;     ///< Available houses
        int hotels;     ///< Available hotels
        int money;      ///< Available money
        std::map<std::string, int> propertyValues; ///< Property price lookup
    };

    ResourceManager<int> houseManager;    ///< Manages house inventory
    ResourceManager<int> hotelManager;    ///< Manages hotel inventory
    ResourceManager<int> moneyManager;    ///< Manages money supply
    Board* board;                         ///< Pointer to game board
    std::map<std::string, int> propertyValues;  ///< Property price mapping
    std::queue<std::shared_ptr<Property>> foreclosedProperties;  ///< Properties in foreclosure
    std::map<std::string, std::set<std::shared_ptr<Property>>> developedProperties;  ///< Properties with developments
    GameState<BankState> stateManager;    ///< Manages bank state history

    static const int INITIAL_HOUSES = 32;   ///< Starting house count
    static const int INITIAL_HOTELS = 12;   ///< Starting hotel count
    static const int INITIAL_MONEY = 20580; ///< Starting money amount

public:
    /** @brief Default constructor initializing bank resources */
    Bank();  

    /**
     * @brief Sets the game board reference
     * @param b Pointer to game board
     */
    void setBoard(Board* b) { board = b; }

    /**
     * @brief Gets the game board reference
     * @return Pointer to game board
     */
    Board* getBoard() const { return board; }
    
    /**
     * @brief Processes property sale to player
     * @param buyer Player purchasing property
     * @param property Property being purchased
     * @return Transaction success
     */
    bool sellProperty(Player& buyer, std::shared_ptr<Property> property);

    /**
     * @brief Conducts property auction
     * @param players Vector of participating players
     * @param property Property being auctioned
     * @return Auction success
     */
    bool auctionProperty(std::vector<std::reference_wrapper<Player>>& players, std::shared_ptr<Property> property);

    /**
     * @brief Processes house purchase
     * @param player Player buying house
     * @param property Property for development
     * @return Purchase success
     */
    bool buyHouse(Player& player, std::shared_ptr<StandardProperty> property);

    /**
     * @brief Processes hotel purchase
     * @param player Player buying hotel
     * @param property Property for development
     * @return Purchase success
     */
    bool buyHotel(Player& player, std::shared_ptr<StandardProperty> property);

    /**
     * @brief Returns house to bank inventory
     * @param property Property removing house
     */
    void returnHouse(std::shared_ptr<StandardProperty> property);

    /**
     * @brief Returns hotel to bank inventory
     * @param property Property removing hotel
     */
    void returnHotel(std::shared_ptr<StandardProperty> property);

    /**
     * @brief Handles player bankruptcy
     * @param bankruptPlayer Player declaring bankruptcy
     * @param creditorPlayer Optional creditor player
     */
    void handleBankruptcy(Player& bankruptPlayer, Player* creditorPlayer = nullptr);

    /**
     * @brief Processes property mortgage
     * @param player Player mortgaging property
     * @param property Property being mortgaged
     * @return Mortgage success
     */
    bool mortgageProperty(Player& player, std::shared_ptr<Property> property);

    /**
     * @brief Processes property unmortgage
     * @param player Player unmortgaging property
     * @param property Property being unmortgaged
     * @return Unmortgage success
     */
    bool unmortgageProperty(Player& player, std::shared_ptr<Property> property);
    
    /** @brief Saves current bank state */
    void saveState();

    /** @brief Reverts to previous bank state */
    void undoState();

private:
    /**
     * @brief Validates player can afford transaction
     * @param player Player to check
     * @param amount Transaction amount
     * @return Transaction validity
     */
    bool validateTransaction(const Player& player, int amount) const;

    /** @brief Initialize property prices */
    void initializePropertyValues();

    /**
     * @brief Process property foreclosure
     * @param player Owner of foreclosed property
     * @param property Property being foreclosed
     */
    void foreclosureProperty(Player& player, std::shared_ptr<Property> property);

    /**
     * @brief Check if property can be developed
     * @param player Property owner
     * @param property Property to develop
     * @return Development possibility
     */
    bool canDevelop(const Player& player, std::shared_ptr<StandardProperty> property) const;

    /**
     * @brief Calculate property value
     * @param property Property to assess
     * @return Property value
     */
    int assessPropertyValue(const std::shared_ptr<Property>& property) const;
};
#endif