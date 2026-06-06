#pragma once

#include <string>
#include <memory>
#include <deque>

#include "Character.hpp"

namespace console {
    void clearScreen();

    int readInteger();

    int readIntInRange(int lower, int upper);

    Character* queryCharacter(std::deque<std::unique_ptr<Character>>& characters);

    void pressEnterToContinue();

    void printAndWait(std::string_view message, int ms);

    // set "end" to '\0' if you don't want a specific terminating character
    void slowPrint(std::string_view message, int ms = 20, char end='\n');
    // set "end" to '\0' if you don't want a specific terminating character
    void slowPrintAndWait(std::string_view message, int ms_interval = 20, int ms_waitTime = 200, char end='\n');

    void waitForMilliseconds(int ms = 2000);

    void printHealthBar(float hp);
};