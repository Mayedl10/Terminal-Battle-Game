#pragma once

#include <vector>

#include "Tile.hpp"
#include "Game.hpp"

class Level {
    std::vector<std::vector<Tile>> map;

public:

    void displayLevel(Game *game);

    Level();
};