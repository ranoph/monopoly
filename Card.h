#ifndef CARD_H
#define CARD_H
#include <string>
#include <functional>

/**
 * @brief Represents a Chance or Community Chest card
 */
class Card {
private:
    std::string description;                ///< Card text
    std::function<void(Player&)> action;    ///< Card effect
    bool keepable;                          ///< Can be kept for later use

public:
    /**
     * @brief Constructs a new Card
     * @param desc Card description text
     * @param act Function to execute when card is drawn
     * @param keep Whether card can be kept (default: false)
     */
    Card(const std::string& desc, std::function<void(Player&)> act, bool keep = false)
        : description(desc), action(act), keepable(keep) {}
    
    /**
     * @brief Executes card's action
     * @param player Player drawing the card
     */
    void execute(Player& player) const { action(player); }

    /**
     * @brief Checks if card can be kept
     * @return true if card can be kept
     */
    bool isKeepable() const { return keepable; }

    /**
     * @brief Gets card description
     * @return Card text
     */
    const std::string& getDescription() const { return description; }
};
#endif