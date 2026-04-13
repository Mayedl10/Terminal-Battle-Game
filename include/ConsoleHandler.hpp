#pragma once

#include <string>

#include "Game.hpp"
#include "Level.hpp"

enum OutputColours {

};

class ConsoleHandler {
public:
    static void clearScreen();
    static void printLevel(Game *game, Level *level);
    static void printColour(std::string message, OutputColours col);

    static int readInteger();

};