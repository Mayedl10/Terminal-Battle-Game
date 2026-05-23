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

    static std::unique_ptr<Character>& queryCharacter(std::deque<std::unique_ptr<Character>>& characters);

    static void pressEnterToContinue();

};