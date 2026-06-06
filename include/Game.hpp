#pragma once

#include <vector>
#include <string>
#include <deque>
#include <memory>
#include <random>

#include "Character.hpp"
#include "Level.hpp"
#include "ActionHandler.hpp"
#include "AIHandler.hpp"

struct GameConfig;

class Game {
    std::deque<std::unique_ptr<Character>> characters;
    std::vector<std::unique_ptr<Level>> levels;
    int levelIdx;   // index for current level
    std::mt19937 rng;
    std::unique_ptr<AIHandler> aiHandler;
    std::unique_ptr<ActionHandler> actionHandler;
    
    void loadLevels(const std::string& levelFolderPath);
    void loadPlayers(const int characterCount, const int aiCharacterCount);
    void selectRandomLevel();
    void placePlayers();
    void enqueueFrontCharacter(); // moves the character at the front of the player deque to the back
    void distributeItems(); // distributes items randomly across the map at the beginning of the game
    
    Level* getLevel();
    void setLevelIdx(int idx);
    int getLevelIdx();
    std::mt19937& getRNG();
    
public:
    
  
    // returns false if the game is over
    bool runGameCycle();
    void winScreen();

    /*
    * levelFolderPath ... path to a folder that contains the levels
    * characters are sorted according to their speed; they then get to take action in order until the game ends
    */
    Game(const std::string& levelFolderPath, const int characterCount, const int AIcharacterCount);
    Game(const GameConfig& config);
};