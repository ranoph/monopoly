#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H
#include <string>
#include <stdexcept>

/**
 * @brief Exception for insufficient player funds
 */
class InsufficientFundsException : public GameException {
public:
    /**
     * @brief Constructs exception with error message
     * @param message Description of funds error
     */
    explicit InsufficientFundsException(const std::string& message) : GameException(message) {}
};
#endif