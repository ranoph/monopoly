#ifndef DICE_H
#define DICE_H
#include <random>
#include <utility>

/**
 * @brief Simulates a pair of dice for the game
 */
class Dice {
private:
    std::random_device rd;                          ///< Random device for seeding
    std::mt19937 gen;                              ///< Random number generator
    std::uniform_int_distribution<> dis;           ///< Distribution for die rolls
    int lastRoll1;                                 ///< Last value of first die
    int lastRoll2;                                 ///< Last value of second die
    int doublesCount;                              ///< Count of consecutive doubles

public:
    /**
     * @brief Constructor initializing random number generation
     */
    Dice();

    /**
     * @brief Rolls both dice
     * @return Pair of values for both dice
     */
    std::pair<int, int> roll();

    /**
     * @brief Checks if last roll was doubles
     * @return true if dice showed same value
     */
    bool isDoubles() const;

    /**
     * @brief Gets sum of last roll
     * @return Total of both dice
     */
    int getTotal() const;

    /**
     * @brief Resets doubles counter
     */
    void resetDoubles();

    /**
     * @brief Gets number of consecutive doubles
     * @return Count of doubles rolled
     */
    int getDoublesCount() const;
};
#endif