#pragma once

#include <vector>
#include <string>
#include <queue>

#include "Character.hpp"
#include "Level.hpp"

class Game {
    std::queue<Character*> characters;
    std::vector<Level*> levels;
    int levelIdx;   // index for current level

public:

    void runGameCycle();

    int getLevelIdx();
    void setLevelIdx(int idx);

    /*
    * levelFolderPath ... path to a folder that contains the levels
    * characters are sorted according to their speed; they then get to take action in order until the game ends
    */
    Game(std::string levelFolderPath);
    ~Game();
};