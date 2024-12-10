#ifndef MONOPOLY_GAME_H
#define MONOPOLY_GAME_H
#include "Board.h"
#include "Bank.h"
#include "Dice.h"
#include <vector>
#include <memory>

/**
 * @brief Main game controller class
 */
class MonopolyGame {
private:
    std::vector<std::unique_ptr<Player>> players;  ///< Game players
    std::unique_ptr<Board> board;                  ///< Game board
    std::unique_ptr<Bank> bank;                    ///< Game bank
    Dice dice;                                     ///< Game dice
    bool gameOver;                                 ///< Game state flag
    int currentPlayerIndex;                        ///< Active player index

    /**
     * @brief Set up initial players
     */
    void initializePlayers();

    /**
     * @brief Execute single player turn
     */
    void playTurn();

    /**
     * @brief Handle player in jail
     * @param player Jailed player
     */
    void handleJailTurn(Player& player);

    /**
     * @brief Handle normal player turn
     * @param player Current player
     */
    void handleRegularTurn(Player& player);

    /**
     * @brief Handle post-turn options
     * @param player Current player
     */
    void handlePostTurnActions(Player& player);

    /**
     * @brief Display/manage player properties
     * @param player Property owner
     */
    void manageProperties(Player& player);

    /**
     * @brief Handle property trading
     * @param player Trading player
     */
    void handleTrade(Player& player);

    /**
     * @brief Handle property development
     * @param player Developing player
     */
    void handleDevelopment(Player& player);

    /**
     * @brief Handle property mortgaging
     * @param player Mortgaging player
     */
    void handleMortgage(Player& player);

    /**
     * @brief Process player bankruptcy
     * @param player Bankrupt player
     */
    void handleBankruptcy(Player& player);

    /**
     * @brief Advance to next player
     */
    void nextPlayer();

    /**
     * @brief Display game winner
     */
    void announceWinner();

public:
    /**
     * @brief Initialize game components
     */
    MonopolyGame();

    /**
     * @brief Start and run game
     */
    void playGame();
};
#endif