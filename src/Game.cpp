#include <vector>
#include <string>
#include <deque>
#include <filesystem>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <utility>

#include "Game.hpp"
#include "Character.hpp"
#include "Level.hpp"
#include "Classes.hpp"
#include "Tile.hpp"
#include "InputQuery.hpp"

// returns false if the game is over
bool Game::runGameCycle() {
    int aliveCount = 0;
    for (auto& ch: characters) {
        if (ch->getHealth() > 0.0) {
            aliveCount++;
        }
    }
    // exit if the game is over
    if (aliveCount < 1) {
        return false;
    }

    // query front-of-queue character for an action or make AI decide

    // move front-of-queue character to the back

    // no need to check if the game is over, the next cycle deals with that
    return true;
}

std::mt19937& Game::getRNG() {
    return rng;
}


int Game::getLevelIdx() {
    return levelIdx;
}

void Game::setLevelIdx(int idx) {
    levelIdx = idx;
}

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
        std::cerr << "An exception occurred while trying to load the levels. Game::Game() tried to create a list of all files in \""
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
            std::cerr << "An exception occurred while trying to load level: " << levelFile << "\n"
            << e.what() << std::endl;;
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

    for (int i = 0; i < characterCount; i++) {
        if (i >= AIcharacterCount) {

            std::cout << "Player " << (char)(nameCounter) << ", please select a character class." << std::endl;
            CharacterClass cclass = (CharacterClass)classQuery.query();

            characters.push_back(std::make_unique<Character>(
                nameCounter++,  // increment player counter
                cclass,
                true
            ));
        } else {
            // this is an AI
            characters.push_back(std::make_unique<Character>(
                nameCounter++,  // increment player counter
                Character::validClasses[classDistribution(getRNG())],    // random class, very hacky solution
                false
            ));
        }

    }
}

void Game::selectRandomLevel() {
    // unloads all levels that don't support enough players!
    // looping backwards to avoid shifting indexes
    for (int i = levels.size()-1; i >= 0; i--) {
        if (levels[i]->getMaxSupportedCharacters() < static_cast<int>(characters.size())) {
            levels.erase(levels.begin() + i);
        }
    }

    if (levels.size() <= 0) {
        throw std::runtime_error("Game::selectRandomLevel: no provided level can hold enough characters");
    }

    std::uniform_int_distribution<int> classDistribution(0, levels.size()-1);
    this->setLevelIdx(classDistribution(getRNG()));
}

void Game::placePlayers() {
    std::vector<std::pair<int, int>> validPositions;
    auto& map = levels[getLevelIdx()]->getMap();

    for (int i = 0; i < static_cast<int>(map.size()); i++) {
        for (int j = 0; j < static_cast<int>(map[i].size()); j++) {
            if (map[i][j].type == TileType::TT_CharacterSpawn) {
                validPositions.push_back({i, j});
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

Game::Game(const std::string& levelFolderPath, const int characterCount, const int humanCharacterCount) {

    // seed random number engine with unix epoch
    rng = std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    loadLevels(levelFolderPath);
    loadPlayers(characterCount, humanCharacterCount);

    selectRandomLevel();
    placePlayers();

    /*
    * sort characters by speed
    * 
    * an std::deque is used, so i can a) sort it and b) treat it like a regular queue
    * this way, I can always handle only the character at the front for each turn, without
    * having to worry about weird indexing shenanigans
    */
    std::sort(characters.begin(), characters.end(), 
        [](const std::unique_ptr<Character>& a, const std::unique_ptr<Character>& b) {
            return (a->getSpeed() > b->getSpeed()); // high speed comes first
        }
    );

    levels[getLevelIdx()]->displayLevel(characters);
}

Game::~Game() {

    // TODO
}