#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <stdexcept>
#include <string>

/**
 * @brief Template class for managing game resources
 * @tparam T Resource type
 */
template<typename T>
class ResourceManager {
private:
    T current;                ///< Current amount
    T maximum;                ///< Maximum capacity
    std::string resourceName; ///< Resource identifier

public:
    /**
     * @brief Constructor
     * @param initial Initial amount
     * @param max Maximum capacity
     * @param name Resource identifier
     * @throws std::invalid_argument if initial > max
     */
    ResourceManager(T initial, T max, const std::string& name) 
        : current(initial), maximum(max), resourceName(name) {
        if (initial > max) {
            throw std::invalid_argument("Initial amount cannot exceed maximum");
        }
    }

    /**
     * @brief Consume resource amount
     * @param amount Amount to consume
     * @return Success status
     */
    bool consume(T amount) {
        if (amount > current) {
            return false;
        }
        current -= amount;
        return true;
    }

    /**
     * @brief Add resource amount
     * @param amount Amount to add
     * @return Success status
     */
    bool add(T amount) {
        if (current + amount > maximum) {
            return false;
        }
        current += amount;
        return true;
    }

    /**
     * @brief Transfer resources between managers
     * @param other Receiving manager
     * @param amount Amount to transfer
     * @return Success status
     */
    bool transfer(ResourceManager<T>& other, T amount) {
        if (consume(amount)) {
            if (other.add(amount)) {
                return true;
            }
            add(amount);
        }
        return false;
    }

    T getCurrent() const { return current; }         ///< Get current amount
    T getMaximum() const { return maximum; }         ///< Get maximum capacity
    T getAvailable() const { return maximum - current; } ///< Get available capacity
    bool isFull() const { return current == maximum; }   ///< Check if full
    bool isEmpty() const { return current == 0; }        ///< Check if empty
};
#endif