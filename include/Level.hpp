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

public:

    std::vector<std::vector<Tile>>& getMap();

    void displayLevel(std::deque<std::unique_ptr<Character>>& players);

    Level(std::string levelFilePath);
};