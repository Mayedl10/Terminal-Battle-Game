#include "Level.hpp"

#include <vector>
#include <string>

#include "Tile.hpp"
#include "Game.hpp"

std::vector<std::vector<Tile>>& Level::getMap() {
    return map;
}

void Level::displayLevel(Game *game) {
    // TODO
}

Level::Level(std::string levelFilePath) {
    // TODO: load map based on file path
}