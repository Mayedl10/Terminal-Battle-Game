#include <iostream>

#include "Game.hpp"
#include "ConsoleUtils.hpp"

int main(int argc, char **argv) {
    // fixes an annoying bug where the OS's input stream didn't match the program's.
    // this caused console::pressEnterToContinue to not work properly when
    // called multiple times in a row
    // note: this makes iostream no longer thread safe and detaches CPP's buffers
    // from C's buffers (ie. use mutexes around IO operations and dont use C's IO stuff)
    std::ios_base::sync_with_stdio(false);
    
    try {
        Game game("./leveldata/debug/", 2, 1);
        while (game.runGameCycle()) { }
        game.winScreen();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';

        std::cout << "[[AN ERROR OCCURRED AND THE GAME WAS CLOSED]]" << std::endl;
        console::pressEnterToContinue();
        return 1;
    }

    console::pressEnterToContinue();
    return 0;
}