#include "Game.hpp"

#include <iostream>
#include <memory>

#include "Character.hpp"
#include "ConsoleHandler.hpp"

void Game::winScreen() {
    Character *winner;
    for (auto& ch: characters) {
        if (ch->isAlive()) {
            winner = ch.get();
            break;
        }
    }

    ConsoleHandler::slowPrintAndWait("--- GAME OVER ---", 50);
    ConsoleHandler::slowPrintAndWait("Congratulations, Player " + std::string(1, winner->getNameUpper()) + (winner->isHuman() ? "." : " (AI)."));
    ConsoleHandler::slowPrintAndWait("You won the game!", 20, 1000);
}