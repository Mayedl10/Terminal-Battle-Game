#pragma once

#include <vector>
#include <string>

#include "Tile.hpp"

constexpr std::string_view levelFileExtension = ".dat";

class Game;

class Level {
    std::vector<std::vector<Tile>> map;

public:

    std::vector<std::vector<Tile>>& getMap();

    void displayLevel(Game *game);

    Level(std::string levelFilePath);
};