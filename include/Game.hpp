#pragma once

#include <vector>
#include <string>
#include <deque>
#include <string_view>
#include <memory>
#include <random>
#include <utility>

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
    void enqueueFrontCharacter(); // moves the character at the front of the player deque to the back

    // returns false if the attack failed
    // if it succeeds, the function applies damage automatically
    bool attemptAttack(std::unique_ptr<Character>& attacker, std::unique_ptr<Character>& target);
    void characterAction(std::unique_ptr<Character>& character, QueryOptionsCharacterAction action);
    void moveCharacter(std::unique_ptr<Character>& character, QueryOptionsCharacterAction direction, int distance);

    // weights are defined in enum ItemSpawnWeights
    ItemType getRandomItemFromWeights();
    // distributes items randomly across the map at the beginning of the game
    void distributeItems();

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
};