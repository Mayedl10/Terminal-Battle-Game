#include <vector>
#include <string>
#include <deque>
#include <filesystem>
#include <iostream>

#include "Game.hpp"
#include "Character.hpp"
#include "Level.hpp"


void Game::runGameCycle() {

}

int Game::getLevelIdx() {
    return levelIdx;
}

void Game::setLevelIdx(int idx) {
    levelIdx = idx;
}

Game::Game(std::string levelFolderPath) {
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
        levels.push_back(std::make_unique<Level>(levelFile));
    }

    for (const auto& level: levels) {
        level->displayLevel(nullptr);
    }
}

Game::~Game() {
    // TODO
}