#pragma once

#include <string>

enum OutputColours {

};

class ConsoleHandler {
public:
    static void clearScreen();

    static int readInteger();

    static int readIntInRange(int lower, int upper);

    static void printColour(std::string message, OutputColours col);

};