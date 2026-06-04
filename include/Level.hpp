#pragma once

#include <vector>
#include <string>
#include <memory>
#include <deque>

#include "Tile.hpp"

class Character;

constexpr std::string_view levelFileExtension = ".dat";

class Game;

class Level {
    // note: tiles are stored with inverted indexes
    // map[y][x] returns the tile at position (x, y)
    std::vector<std::vector<Tile>> map; 
    int maxSupportedCharacters;

public:

    std::vector<std::vector<Tile>>& getMap();
    int getMaxSupportedCharacters();

    void displayLevel(std::deque<std::unique_ptr<Character>>& players);
    TileType getTileTypeAt(int x, int y);
    Tile& getTileAt(int x, int y);
    int getWidth();
    int getHeight();

    Level(std::string levelFilePath);
};