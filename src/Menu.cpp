#include "Game.hpp"

#include <iostream>
#include <memory>

#include "Character.hpp"

void Game::winScreen() {
    Character *winner;
    for (auto& ch: characters) {
        if (ch->isAlive()) {
            winner = ch.get();
            break;
        }
    }

    std::cout << "--- GAME OVER ---\n"
    << "Congratulations, Player " << winner->getNameUpper() << (winner->isHuman() ? "." : " (AI).")
    << "\nYou won the game!" << std::endl;
}