#include <iostream>

#include "Game.hpp"
#include "Level.hpp"
#include "ConsoleHandler.hpp"

int main(int argc, char **argv) {

    Game test("./data/levels/", 2, 1);
    
    
    
    try {
        while (test.runGameCycle()) { }
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';

        std::cout << "[[AN ERROR OCCURRED AND THE GAME WAS CLOSED]]" << std::endl;
    }

    ConsoleHandler::pressEnterToContinue();
    
}