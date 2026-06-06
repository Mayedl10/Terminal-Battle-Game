#pragma once

#include <string>
#include <memory>
#include <deque>

#include "Character.hpp"

class ConsoleHandler {
public:
    static void clearScreen();

    static int readInteger();

    static int readIntInRange(int lower, int upper);

    static Character* queryCharacter(std::deque<std::unique_ptr<Character>>& characters);

    static void pressEnterToContinue();

    static void printAndWait(std::string_view message, int ms);

    // set "end" to '\0' if you don't want a specific terminating character
    static void slowPrint(std::string_view message, int ms = 20, char end='\n');
    // set "end" to '\0' if you don't want a specific terminating character
    static void slowPrintAndWait(std::string_view message, int ms_interval = 20, int ms_waitTime = 200, char end='\n');

    static void waitForMilliseconds(int ms = 2000);

    static void printHealthBar(float hp);
};