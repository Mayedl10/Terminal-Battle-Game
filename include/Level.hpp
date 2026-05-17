#pragma once

#include <vector>
#include <string>
#include <memory>
#include <deque>

#include "Tile.hpp"
#include "Character.hpp"

constexpr std::string_view levelFileExtension = ".dat";

class Game;

class Level {
    std::vector<std::vector<Tile>> map;
    int maxSupportedCharacters;

public:

    std::vector<std::vector<Tile>>& getMap();
    int getMaxSupportedCharacters();

    void displayLevel(std::deque<std::unique_ptr<Character>>& players);
    TileType getTileTypeAt(int x, int y);
    int getWidth();
    int getHeight();

    Level(std::string levelFilePath);
};