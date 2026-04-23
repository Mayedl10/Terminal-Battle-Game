#include <vector>
#include <string>
#include <queue>

#include "Game.hpp"
#include "Character.hpp"
#include "Level.hpp"


void Game::runGameCycle() {

}

int Game::getLevelIdx() {
    return levelIdx;
}

void Game::setLevelIdx(int idx) {
    levelIdx = idx;
}

Game::Game(std::string levelFolderPath) {
    // TODO
}

Game::~Game() {
    // TODO
}