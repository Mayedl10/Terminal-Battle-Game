#include "Game.hpp"

#include <iostream>
#include <memory>

#include "Character.hpp"
#include "ConsoleUtils.hpp"

void Game::winScreen() {
    Character *winner;
    for (auto& ch: characters) {
        if (ch->isAlive()) {
            winner = ch.get();
            break;
        }
    }

    console::slowPrintAndWait("--- GAME OVER ---", 50);
    console::slowPrintAndWait("Congratulations, Player " + std::string(1, winner->getNameUpper()) + (winner->isHuman() ? "." : " (AI)."));
    console::slowPrintAndWait("You won the game!", 20, 1000);
}