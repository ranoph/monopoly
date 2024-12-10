#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <vector>
#include <memory>
#include <stdexcept>

/**
 * @brief Template class managing game state history
 * @tparam StateData Type of state data to store
 */
template<typename StateData>
class GameState {
private:
    std::vector<StateData> history;    ///< Vector of state history
    size_t currentIndex;               ///< Current position in history

public:
    /**
     * @brief Default constructor
     */
    GameState() : currentIndex(0) {}

    /**
     * @brief Saves new game state
     * @param state State data to save
     */
    void saveState(const StateData& state) {
        if (currentIndex < history.size()) {
            history.erase(history.begin() + currentIndex, history.end());
        }
        history.push_back(state);
        currentIndex++;
    }

    /**
     * @brief Checks if undo is possible
     * @return true if states exist to undo
     */
    bool canUndo() const {
        return currentIndex > 0;
    }

    /**
     * @brief Checks if redo is possible
     * @return true if states exist to redo
     */
    bool canRedo() const {
        return currentIndex < history.size();
    }

    /**
     * @brief Reverts to previous state
     * @return Reference to previous state
     * @throws std::runtime_error if no states to undo
     */
    StateData& undo() {
        if (!canUndo()) {
            throw std::runtime_error("No states to undo");
        }
        return history[--currentIndex];
    }

    /**
     * @brief Advances to next state
     * @return Reference to next state
     * @throws std::runtime_error if no states to redo
     */
    StateData& redo() {
        if (!canRedo()) {
            throw std::runtime_error("No states to redo");
        }
        return history[currentIndex++];
    }

    /**
     * @brief Gets current state
     * @return Reference to current state
     * @throws std::runtime_error if no state exists
     */
    StateData& getCurrentState() {
        if (history.empty()) {
            throw std::runtime_error("No state available");
        }
        return history[currentIndex - 1];
    }
};
#endif