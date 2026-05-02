#pragma once

#include <string>
#include <iostream>ss

#include "Game.hpp"
#include "Level.hpp"

enum OutputColours {

};

class ConsoleHandler {
public:
    static void clearScreen() {
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

    static void printColour(std::string message, OutputColours col);

    static int readInteger();

};