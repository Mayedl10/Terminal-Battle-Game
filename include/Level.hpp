#pragma once

#include <vector>
#include <string>

#include "Tile.hpp"
#include "Game.hpp"

class Level {
    std::vector<std::vector<Tile>> map;

public:

    std::vector<std::vector<Tile>>& getMap();

    void displayLevel(Game *game);

    Level(std::string levelFilePath);
};