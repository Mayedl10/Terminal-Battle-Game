#pragma once

#include <string>
#include <memory>
#include <deque>

#include "Character.hpp"


namespace console {
    namespace style {
        // https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
        constexpr std::string_view Reset     = "\033[0m";
        constexpr std::string_view Black     = "\033[30m";
        constexpr std::string_view Red       = "\033[31m";
        constexpr std::string_view Green     = "\033[32m";
        constexpr std::string_view Yellow    = "\033[33m";
        constexpr std::string_view Blue      = "\033[34m";
        constexpr std::string_view Magenta   = "\033[35m";
        constexpr std::string_view Cyan      = "\033[36m";
        constexpr std::string_view White     = "\033[37m";
        constexpr std::string_view Bold      = "\033[1m";

    };

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

    void printColouredChar(char c, std::string_view col);
    void printColouredCharBold(char c, std::string_view col);
};