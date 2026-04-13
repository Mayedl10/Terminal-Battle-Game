#pragma once

#include <vector>
#include <string>

#include "Character.hpp"
#include "Level.hpp"

class Game {
    std::vector<Character*> characters;
    std::vector<Level*> levels;
    int levelIdx;   // index for current level

public:

    /*
    * levelFolderPath ... path to a folder that contains the levels
    */
    Game(std::string levelFolderPath);
    ~Game();
};