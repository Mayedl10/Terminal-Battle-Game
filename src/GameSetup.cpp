#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <deque>
#include <string>
#include <random>
#include <algorithm>

#include "Game.hpp"

// this file holds implementations of Game members that are used during setup

void Game::loadLevels(const std::string& levelFolderPath) {
    // get list of all files in folder that have the matching extension (defined in Level.hpp)
    std::vector<std::string> files;
    try {
        for (const auto& entry: std::filesystem::directory_iterator(levelFolderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == levelFileExtension) {
                files.push_back(entry.path().string());
            }
        }
    } catch(const std::filesystem::filesystem_error& e) {
        std::cerr << "Game::loadLevels: An exception occurred while trying to load the levels. Game::Game() tried to create a list of all files in \""
        << levelFolderPath << "\" with extension " << levelFileExtension << " but failed. Exception thrown:\n" 
        << e.what() << std::endl;
        throw; // re-throw exception
    }
        
    // initialise levels
    for (const auto& levelFile: files) {
        try {
            levels.push_back(std::make_unique<Level>(levelFile));
        }
        catch(const std::exception& e) {
            std::cerr << "Game::loadLevels: An exception occurred while trying to load level: " << levelFile << "\n"
            << e.what() << std::endl;
            throw;
        } 
    }

}

void Game::loadPlayers(const int characterCount, const int AIcharacterCount) {
    if (characterCount <= AIcharacterCount) {
        throw std::invalid_argument("Game::loadPlayers: AIcharacterCount exceeds total characterCount");
    }

    if (characterCount > 26) {
        throw std::invalid_argument("Game::loadPlayers: cannot create more than 26 characters (A-Z)");
    }

    char nameCounter = 'A';
    std::uniform_int_distribution<int> classDistribution(0, 3);
    
    InputQuery classQuery = InputQuery({
        {"Fighter", true, CharacterClass::CC_Fighter},
        {"Sorcerer", true, CharacterClass::CC_Mage},
        {"Rogue", true, CharacterClass::CC_Rogue},
        {"Ranger", true, CharacterClass::CC_Ranger}
    });

    int humanPlayerCount = characterCount - AIcharacterCount;

    for (int i = 0; i < characterCount; i++) {
        if (i >= humanPlayerCount) {
            // this is an AI
            characters.push_back(std::make_unique<Character>(
                nameCounter++,  // increment player counter
                Character::validClasses[classDistribution(getRNG())],    // random class, very hacky solution
                false
            ));

        } else {
            // this is a human
            std::cout << "Player " << static_cast<char>(nameCounter) << ", please select a character class." << std::endl;
            CharacterClass cclass = static_cast<CharacterClass>(classQuery.query());
    
            characters.push_back(std::make_unique<Character>(
                nameCounter++,  // increment player counter
                cclass,
                true
            ));
        }

    }
}

void Game::selectRandomLevel() {
    // unloads all levels that don't support enough players!
    // looping backwards to avoid shifting indexes
    for (int i = static_cast<int>(levels.size())-1; i >= 0; i--) {
        if (levels[i]->getMaxSupportedCharacters() < static_cast<int>(characters.size())) {
            levels.erase(levels.begin() + i);
        }
    }

    if (levels.size() <= 0) {
        throw std::runtime_error("Game::selectRandomLevel: no provided level can hold enough characters");
    }

    std::uniform_int_distribution<int> classDistribution(0, static_cast<int>(levels.size())-1);
    this->setLevelIdx(classDistribution(getRNG()));
}

void Game::placePlayers() {
    std::vector<std::pair<int, int>> validPositions;
    auto& map = getLevel()->getMap();

    for (int i = 0; i < static_cast<int>(map.size()); i++) {
        for (int j = 0; j < static_cast<int>(map[i].size()); j++) {
            if (map[i][j].type == TileType::TT_CharacterSpawn) {
                validPositions.push_back({j, i});
            }
        }
    }

    // minimum number of spawn points doesn't need to be checked here because it's already guaranteed by Game::selectRandomLevel

    // shuffle valid positions
    std::shuffle(validPositions.begin(), validPositions.end(), getRNG());

    // in (shuffled) order, assign character positions
    for (int i = 0; i < static_cast<int>(characters.size()); i++) {
        characters[i]->setXpos(validPositions[i].first);
        characters[i]->setYpos(validPositions[i].second);
    }
}