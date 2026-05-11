#include "ConsoleHandler.hpp"

#include <string>
#include <iostream>
#include <limits>

void ConsoleHandler::clearScreen() {
    // ANSI escape sequence that clears the screen and then resets the cursor position
    // SHOULD work cross platform, but really depends on the terminal used
    std::cout << "\033[2J\033[1;1H";

    // gnome-terminal:
    //      works, but you can scroll "up" to before the clear
    // vscode integrated terminal:
    //      same as gnome-terminal
    // tty on linux mint:
    //      works
}

int ConsoleHandler::readInteger() {
    int val;
    while (true) {
        std::cout << "> ";
        if (std::cin >> val) break; // successful read -> break, continue if input is invalid
        std::cin.clear();   // clear fail state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard line
        std::cout << "Input must be a valid integer. Please try again." << std::endl;
    }
    return val;
}

int ConsoleHandler::readIntInRange(int lower, int upper) {
    while (true) {
        int choice = readInteger();
        if (choice >= lower && choice <= upper) return choice;
        std::cout << "Input must be a number >= " << lower << " and <= " << upper << std::endl;
    }
}