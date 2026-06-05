#include <vector>
#include <string>
#include <deque>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <utility>
#include <cmath>
#include <optional>
#include <variant>

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
    Character *current = characters.front().get();

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
        // print character HP
        std::cout << "\t " << p->getNameUpper() << ": " << p->getHealth() << "HP" << std::endl;
    }

    // hide/show "use item" actions
    current->getQueryObject().setVisibility(QueryOptionsCharacterAction::USE_ITEM, current->hasItem());

    // the following two if statements could be merged into one, but the
    // current structure makes the control flow easier to understand

    // query front-of-queue character for an action or make AI decide
    QueryOptionsCharacterAction choice;
    std::optional<std::variant<Character*, int>> secondAIParameter;
    
    if (current->isHuman()) {
        choice = current->pickAction();
    } else {
        auto aiChoice = pickActionAI();
        choice = aiChoice.first;
        secondAIParameter = aiChoice.second;
    }

    // execute actions
    if (current->isHuman()) {
        characterAction(current, choice);
    } else {
        aiCharacterAction(current, choice, secondAIParameter);
        ConsoleHandler::waitForMilliseconds();
    }

    // pick up items
    for (auto& ch: characters) {
        // the variable "pickup" is only used to make the code more "readable"
        bool pickup = ch->attemptPickup(getLevel());
        if (pickup) {
            std::cout << "Player " << ch->getNameUpper() << " picked up an item!" << std::endl;
            ConsoleHandler::waitForMilliseconds();
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

bool Game::attemptAttack(Character* attacker, Character* target) {
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
        ConsoleHandler::waitForMilliseconds();
        return false;
    }

    // if target is in range, perform """raymarching""" to see if there is a wall obstructing the path.
    // use Bresenham's line algorithm

    auto& map = getLevel()->getMap();
    auto line = MathUtils::bresenhamsLineAlgorithm(attPos, tarPos);

    // check if any of the bresenham line points are walls
    // return if any of them are
    for (auto p: line) {
        if (map[p.second][p.first].type == TileType::TT_Wall) {
            std::cout << "The attack failed because there was a wall in the way." << std::endl;
            ConsoleHandler::waitForMilliseconds();
            return false;
        } 
    }

    // attack is valid
    target->hurt(attacker->getStrength());
    std::cout << "Player " << attacker->getNameUpper() << " dealt " << attacker->getStrength() << " points of damage to " << target->getNameUpper() << "!" << std::endl;
    
    // if the attacker is an ai, there was already a sleep caused by Game::runGameCycle
    if (attacker->isHuman()) {
        ConsoleHandler::waitForMilliseconds();
    }

    return true;
}

void Game::characterAction(Character *character, QueryOptionsCharacterAction action) {
    switch (action) {
    case QueryOptionsCharacterAction::ATTACK: {
            // added scope around attack case to prevent a compile time error
            // caused by target being accessible from other cases, even though
            // it might not be initialised there
            std::cout << "Who do you want to attack?" << std::endl;
            Character *target = ConsoleHandler::queryCharacter(characters);
            (void)attemptAttack(character, target);
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
        ConsoleHandler::waitForMilliseconds();
        break;
    default: // if it's none of the above, it's probably a direction. if it isn't, let moveCharacter throw an exception
        // query distance
        std::cout << "How far do you want to go?" << std::endl;
        int dist = ConsoleHandler::readIntInRange(1, character->getSpeed());
        moveCharacter(character, action, dist);
        break;
    }
}

void Game::moveCharacter(Character *character, QueryOptionsCharacterAction direction, int distance) {
    if (distance <= 0 || distance > character->getSpeed()) {
        throw std::invalid_argument(
            "Game::moveCharacter: invalid argument <distance>: " + std::to_string(distance)
            + " is out of range for <character>: " + std::to_string(character->getNameUpper()) 
            + " with maximum <speed>: " + std::to_string(character->getSpeed())
        );
    }
    
    Level *level = getLevel();

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
    std::cout << "Player " << character->getNameUpper() << " moved to (" << character->getXpos() << ", " << character->getYpos() << ")" << std::endl;
}

Game::Game(const std::string& levelFolderPath, const int characterCount, const int AIcharacterCount) {
    ConsoleHandler::clearScreen();
    // seed random number engine with unix epoch
    rng = std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    loadLevels(levelFolderPath);
    loadPlayers(characterCount, AIcharacterCount);

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