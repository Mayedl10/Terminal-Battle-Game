#include "ConsoleUtils.hpp"

#include <string>
#include <iostream>
#include <limits>
#include <vector>
#include <cctype>
#include <memory>
#include <deque>
#include <thread>
#include <chrono>
#include <cmath>

void console::clearScreen() {
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

int console::readInteger() {
    int val;
    while (true) {
        std::cout << "> ";
        if (std::cin >> val) break; // successful read -> break, continue if input is invalid
        std::cin.clear();   // clear fail state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard line
        slowPrint("Input must be a valid integer. Please try again.");
    }
    return val;
}

int console::readIntInRange(int lower, int upper) {
    while (true) {
        int choice = readInteger();
        if (choice >= lower && choice <= upper) return choice;
        slowPrint("Input must be a number >= " + std::to_string(lower) + " and <= " + std::to_string(upper));
    }
}

Character* console::queryCharacter(std::deque<std::unique_ptr<Character>>& characters) {
    while (true) {
        std::cout << "> ";

        char choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            slowPrint("Input must be a valid character. Please try again.");
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        const char selectedName = static_cast<char>(std::toupper(static_cast<unsigned char>(choice)));
        if (selectedName < 'A' || selectedName > 'Z') {
            slowPrint("Input must be a letter between A and Z. Please try again.");
            continue;
        }

        for (auto& character: characters) {
            if (character && character->isAlive() && character->getNameUpper() == selectedName) {
                return character.get();
            }
        }

        slowPrint("No living character with that name exists. Please try again.");
    }
}

void console::pressEnterToContinue() {
    std::cout << "Press Enter to continue..." << std::flush;

    std::cin.clear();    
    std::cin.ignore(std::cin.rdbuf()->in_avail());

    std::string dummyBuffer;
    std::getline(std::cin, dummyBuffer);
}

void console::printAndWait(std::string_view message, int ms) {
    std::cout << message << std::flush; // flush to force the character to be displayed
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void console::slowPrint(std::string_view message, int ms, char end) {
    for (char c: message) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
    if (end != '\0') {
        std::cout << end << std::flush;
    }
}

void console::slowPrintAndWait(std::string_view message, int ms_interval, int ms_waitTime, char end) {
    slowPrint(message, ms_interval, end);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms_waitTime));
}

void console::waitForMilliseconds(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void console::printHealthBar(float hp) {
    int count = static_cast<int>(std::ceil(hp/5.0f)); // one "segment" of the health bar ~ 5HP
    std::cout << "[";

    if (count == 1) {
        std::cout << style::Red;
    } else if (count <= 3) {
        std::cout << style::Yellow;
    } else {
        std::cout << style::Green;
    }

    for (int i = 0; i < count; i++) {
        std::cout << '#';
    }
    std::cout << style::Reset;
    std::cout << "]";
}

void console::printColouredChar(char c, std::string_view col) {
    std::cout << col << c << style::Reset;
}

void console::printColouredCharBold(char c, std::string_view col) {
    std::cout << style::Bold << col << c << style::Reset;

}
