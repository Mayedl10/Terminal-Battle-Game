#include <vector>
#include <string>
#include <deque>
#include <filesystem>
#include <iostream>
#include <chrono>

#include "Game.hpp"
#include "Character.hpp"
#include "Level.hpp"
#include "Classes.hpp"

void Game::runGameCycle() {

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

    for (int i = 0; i < characterCount; i++) {
        if (i >= AIcharacterCount) {

            // todo: prompt for class

            characters.push_back(std::make_unique<Character>(
                nameCounter++,  // increment player counter
                CharacterClass::CC_Invalid,
                true
            ));
        } else {
            // this is an AI
            characters.push_back(std::make_unique<Character>(
                nameCounter++,  // increment player counter
                Character::validClasses[classDistribution(rng)],    // random class, very hacky solution
                true
            ));
        }

        characters.push_back(std::make_unique<Character>(
            nameCounter++,  // increment player counter
            CharacterClass::CC_Invalid,
            (i >= AIcharacterCount)
        ));
    }



}

Game::Game(const std::string& levelFolderPath, const int characterCount, const int humanCharacterCount) {

    // seed random number engine with unix epoch
    rng = std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    loadLevels(levelFolderPath);
    loadPlayers(characterCount, humanCharacterCount);

    for (const auto& level: levels) {
        level->displayLevel(characters);
    }
}

Game::~Game() {

    // TODO
}