#include <iostream>

#include "Game.hpp"
#include "Level.hpp"

int main(int argc, char **argv) {

    Game test("./data/levels/", 2, 0);
    
    while (test.runGameCycle()) { }
    
}