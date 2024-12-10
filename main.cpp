#include "MonopolyGame.h"
#include <iostream>
#include <exception>

int main() {
    try {
        // Create an instance of the MonopolyGame class
        MonopolyGame game;

        // Start the game
        game.playGame();
    } catch (const std::exception& e) {
        // Catch and report any run time errors
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1; // Exit with error code 1
    } catch (...) {
        // Catch any other unexpected errors
        std::cerr << "An unknown error occurred." << std::endl;
        return 1; // Exit with error code 1
    }

    return 0; // Exit with success code
}
