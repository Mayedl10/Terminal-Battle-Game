#include "ConsoleHandler.hpp"

#include <string>
#include <iostream>
#include <limits>
#include <vector>
#include <cctype>
#include <memory>
#include <deque>

void ConsoleHandler::clearScreen() {
    // ANSI escape sequence that clears the screen and then resets the cursor position
    // SHOULD work cross platform, but really depends on the terminal used
    std::cout << "\033[2J\033[1;1H" << std::flush;

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

Character* ConsoleHandler::queryCharacter(std::deque<std::unique_ptr<Character>>& characters) {
    while (true) {
        std::cout << "> ";

        char choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input must be a valid character. Please try again." << std::endl;
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        const char selectedName = static_cast<char>(std::toupper(static_cast<unsigned char>(choice)));
        if (selectedName < 'A' || selectedName > 'Z') {
            std::cout << "Input must be a letter between A and Z. Please try again." << std::endl;
            continue;
        }

        for (auto& character: characters) {
            if (character && character->isAlive() && character->getNameUpper() == selectedName) {
                return character.get();
            }
        }

        std::cout << "No living character with that name exists. Please try again." << std::endl;
    }
}

void ConsoleHandler::pressEnterToContinue() {
    std::cout << "Press Enter to continue..." << std::flush;

    std::cin.clear();    
    std::cin.ignore(std::cin.rdbuf()->in_avail());

    std::string dummyBuffer;
    std::getline(std::cin, dummyBuffer);
}