///Number Guessing Game
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int numberToGuess = std::rand() % 100 + 1; // The number to guess (1-100)
    int userGuess = 0; // Variable to store the user's guess
    int attempts = 0; // Counter for the number of attempts

    std::cout << "Welcome to the Number Guessing Game!" << std::endl;
    std::cout << "Try to guess the number between 1 and 100." << std::endl;

    while (userGuess != numberToGuess) {
        std::cout << "Enter your guess: ";
        std::cin >> userGuess;
        attempts++;

        if (userGuess < numberToGuess) {
            std::cout << "Too low! Try again." << std::endl;
        } else if (userGuess > numberToGuess) {
            std::cout << "Too high! Try again." << std::endl;
        } else {
            std::cout << "Congratulations! You've guessed the number " << numberToGuess 
                      << " in " << attempts << " attempts." << std::endl;
        }
    }

    return 0;
}