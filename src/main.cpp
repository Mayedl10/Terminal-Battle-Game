#include <iostream>

#include "Game.hpp"
#include "Level.hpp"
#include "ConsoleHandler.hpp"

int main(int argc, char **argv) {
    // fixes an annoying bug where the OS's input stream didn't match the program's.
    // this caused ConsoleHandler::pressEnterToContinue to not work properly when
    // called multiple times in a row
    // note: this makes iostream no longer thread safe and detaches CPP's buffers
    // from C's buffers (ie. use mutexes around IO operations and dont use C's IO stuff)
    std::ios_base::sync_with_stdio(false);

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