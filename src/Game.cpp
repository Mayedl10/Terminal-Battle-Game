#include <vector>
#include <string>
#include <deque>
#include <filesystem>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <utility>
#include <cmath>

#include "Game.hpp"
#include "Character.hpp"
#include "Level.hpp"
#include "Classes.hpp"
#include "Tile.hpp"
#include "InputQuery.hpp"
#include "MathUtils.hpp"
#include "ConsoleHandler.hpp"

// returns false if the game is over
bool Game::runGameCycle() {
    auto& current = characters.front();

    ConsoleHandler::clearScreen();
    int aliveCount = 0;
    for (auto& ch: characters) {
        if (ch->getHealth() > 0.0f) {
            aliveCount++;
        }
    }
    // exit if the game is over
    if (aliveCount < 2) {
        return false;
    }

    if (!(current->isAlive())) {
        enqueueFrontCharacter();
        return true;
    }

    getLevel()->displayLevel(characters);
    std::cout << "It's player " << current->getNameUpper() << "'s turn!" << std::endl;
    for (auto& p: characters) {
        std::cout << "\t " << p->getNameUpper() << ": " << p->getHealth() << "HP" << std::endl;
    }

    // hide/show "use item" actions
    current->getQueryObject().setVisibility(QueryOptionsCharacterAction::USE_ITEM, current->hasItem());

    // query front-of-queue character for an action or make AI decide
    auto choice = current->pickAction();

    // execute actions
    characterAction(current, choice);

    // pick up items
    for (auto& ch: characters) {
        // the variable "pickup" is only used to make the code more "readable"
        bool pickup = ch->attemptPickup(getLevel());
        if (pickup) {
            std::cout << "Player " << ch->getNameUpper() << " picked up an item!" << std::endl;
            ConsoleHandler::pressEnterToContinue();
        }
    }

    // move front-of-queue character to the back
    // let character move again if status was queried
    if (choice != QueryOptionsCharacterAction::STATUS) {
        enqueueFrontCharacter();
    }

    // no need to check if the game is over, the next cycle deals with that
    return true;
}

bool Game::attemptAttack(std::unique_ptr<Character>& attacker, std::unique_ptr<Character>& target) {
    // note: the following setup would return true (walls aren't connected diagonally)
    /*
    B . . #
    . . # .
    . # . .
    # . . A
    */
    // to prevent this, make walls "thick" when designing levels
    /*
    B . # #
    . # # .
    # # . .
    # . . A
    */

    std::cout << attacker->getNameUpper() << " attempted an attack on " << target->getNameUpper() << "!" << std::endl;

    std::pair<int, int> attPos = {
        attacker->getXpos(),
        attacker->getYpos()
    };
    std::pair<int, int> tarPos = {
        target->getXpos(),
        target->getYpos()
    };

    // return false if target is too far away
    if (!MathUtils::pointInRange(attPos, static_cast<double>(attacker->getRange()), tarPos)) {
        std::cout << "The attack failed because " << target->getNameUpper() << " was too far away." << std::endl;
        return false;
    }

    // if target is in range, perform raymarching to see if there is a wall obstructing the path.
    // use Bresenham's line algorithm

    auto& map = getLevel()->getMap();
    auto line = MathUtils::bresenhamsLineAlgorithm(attPos, tarPos);

    // check if any of the bresenham line points are walls
    // return if any of them are
    for (auto p: line) {
        if (map[p.second][p.first].type == TileType::TT_Wall) {
            std::cout << "The attack failed because there was a wall in the way." << std::endl;
            return false;
        } 
    }

    // attack is valid
    target->hurt(attacker->getStrength());

    return true;
}

void Game::characterAction(std::unique_ptr<Character>& character, QueryOptionsCharacterAction action) {
    switch (action) {
    case QueryOptionsCharacterAction::ATTACK: {
            // added scope around attack case to prevent a compile time error
            // caused by target being accessible from other cases, even though
            // it might not be initialised there
            auto& target = ConsoleHandler::queryCharacter(characters);
            attemptAttack(character, target);
        }
        break;
    case QueryOptionsCharacterAction::STATUS:
        // print some information
        // don't put character to the end of the queue!
        // ^ handled by runGameCycle
        character->printStatus();
        ConsoleHandler::pressEnterToContinue();
        break;
    case QueryOptionsCharacterAction::PASS:
        // do nothing
        // "pass" ... "i'm not doing anything this round"
        break;
    case QueryOptionsCharacterAction::USE_ITEM:
        std::cout << "Player " << character->getNameUpper() << " used an item: [todo - implement proper item printing]" << std::endl;
        character->useHeldItem();
        ConsoleHandler::pressEnterToContinue();
        break;
    default: // if it's none of the above, it's probably a direction. if it isn't, let moveCharacter throw an exception
        // query distance
        int dist = ConsoleHandler::readIntInRange(1, character->getSpeed());
        moveCharacter(character, action, dist);
        break;
    }
}

void Game::moveCharacter(std::unique_ptr<Character>& character, QueryOptionsCharacterAction direction, int distance) {
    if (distance <= 0 || distance > character->getSpeed()) {
        throw std::invalid_argument(
            "Game::moveCharacter: invalid argument <distance>: " + std::to_string(distance)
            + " is out of range for <character>: " + std::to_string(character->getNameUpper()) 
            + " with maximum <speed>: " + std::to_string(character->getSpeed())
        );
    }
    
    auto& level = getLevel();

    switch (direction) {
    case QueryOptionsCharacterAction::MOVE_N:
        while (distance > 0) {
            int nextY = character->getYpos() -1;

            if (nextY < 0) {
                break; // out of bounds
            }
            auto nextTile = level->getTileTypeAt(character->getXpos(), nextY);
            if (nextTile == TileType::TT_Hole || nextTile == TileType::TT_Wall) {
                break; // hit a wall/hole
            }

            // movement is valid
            character->setYpos(nextY);
            distance--;
        }
        break;
    case QueryOptionsCharacterAction::MOVE_E:
        while (distance > 0) {
            int nextX = character->getXpos() +1;

            if (nextX > level->getWidth()-1) {
                break; // out of bounds
            }
            auto nextTile = level->getTileTypeAt(nextX, character->getYpos());
            if (nextTile == TileType::TT_Hole || nextTile == TileType::TT_Wall) {
                break; // hit a wall/hole
            }

            // movement is valid
            character->setXpos(nextX);
            distance--;
        }
        break;
    case QueryOptionsCharacterAction::MOVE_S:
        while (distance > 0) {
            int nextY = character->getYpos() +1;

            if (nextY > level->getHeight()-1) {
                break; // out of bounds
            }
            auto nextTile = level->getTileTypeAt(character->getXpos(), nextY);
            if (nextTile == TileType::TT_Hole || nextTile == TileType::TT_Wall) {
                break; // hit a wall/hole
            }

            // movement is valid
            character->setYpos(nextY);
            distance--;
        }
        break;
    case QueryOptionsCharacterAction::MOVE_W:
        while (distance > 0) {
            int nextX = character->getXpos() -1;

            if (nextX < 0) {
                break; // out of bounds
            }
            auto nextTile = level->getTileTypeAt(nextX, character->getYpos());
            if (nextTile == TileType::TT_Hole || nextTile == TileType::TT_Wall) {
                break; // hit a wall/hole
            }

            // movement is valid
            character->setXpos(nextX);
            distance--;
        }
        break;
    default:
        throw std::invalid_argument("Game::moveCharacter: invalid argument <direction>: " + std::to_string(direction));
        break;
    }
}


// move a character to the back of the queue
void Game::enqueueFrontCharacter() {
    characters.emplace_back(std::move(characters.front()));
    characters.pop_front();
}

std::unique_ptr<Level>& Game::getLevel() {
    return levels[getLevelIdx()];
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

    for (int i = 0; i < characterCount; i++) {
        if (i >= AIcharacterCount) {

            std::cout << "Player " << static_cast<char>(nameCounter) << ", please select a character class." << std::endl;
            CharacterClass cclass = static_cast<CharacterClass>(classQuery.query());

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

Game::Game(const std::string& levelFolderPath, const int characterCount, const int humanCharacterCount) {
    ConsoleHandler::clearScreen();
    // seed random number engine with unix epoch
    rng = std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    loadLevels(levelFolderPath);
    loadPlayers(characterCount, humanCharacterCount);

    selectRandomLevel();
    placePlayers();
    distributeItems();  // randomly spawn items on regular floor tiles 

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

}