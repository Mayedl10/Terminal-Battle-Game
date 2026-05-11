#pragma once

#include <vector>
#include <string>
#include <deque>
#include <string_view>
#include <memory>
#include <random>

#include "Character.hpp"
#include "Level.hpp"

class Game {
    std::deque<std::unique_ptr<Character>> characters;
    std::vector<std::unique_ptr<Level>> levels;
    int levelIdx;   // index for current level
    std::mt19937 rng;

    void loadLevels(const std::string& levelFolderPath);
    void loadPlayers(const int characterCount, const int humanCharacterCount);
    void selectRandomLevel();
    void placePlayers();

public:

    // returns false if the game is over
    bool runGameCycle();

    int getLevelIdx();
    void setLevelIdx(int idx);
    std::mt19937& getRNG();

    /*
    * levelFolderPath ... path to a folder that contains the levels
    * characters are sorted according to their speed; they then get to take action in order until the game ends
    */
    Game(const std::string& levelFolderPath, const int characterCount, const int AIcharacterCount);
    ~Game();
};