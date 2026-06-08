#include <sstream>
#include <iomanip>
#include <optional>
#include <variant>
#include <string>
#include <stdexcept>

#include "ConsoleUtils.hpp"
#include "MathUtils.hpp"
#include "ActionHandler.hpp"
#include "Character.hpp"
#include "Level.hpp"

bool ActionHandler::attemptAttack(Character *attacker, Character *target) {
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

    std::ostringstream ss;

    ss << "Player " << attacker->getNameUpper() << " attempted an attack on Player " << target->getNameUpper() << "!";
    console::slowPrint(ss.str());

    std::pair<int, int> attPos = {
        attacker->getXpos(),
        attacker->getYpos()
    };
    std::pair<int, int> tarPos = {
        target->getXpos(),
        target->getYpos()
    };

    // return false if target is too far away
    if (!math::pointInRange(attPos, static_cast<double>(attacker->getRange()), tarPos)) {
        ss.str("");
        ss.clear();
        ss << "The attack failed because " << target->getNameUpper() << " was too far away." << std::endl;
        console::slowPrintAndWait(ss.str(), 20, 2200);
        return false;
    }

    // if target is in range, perform """raymarching""" to see if there is a wall obstructing the path.
    // use Bresenham's line algorithm

    auto& map = level->getMap();
    auto line = math::bresenhamsLineAlgorithm(attPos, tarPos);

    // check if any of the bresenham line points are walls
    // return if any of them are
    for (auto p: line) {
        if (map[p.second][p.first].type == TileType::TT_Wall) {
            ss.str("");
            ss.clear();
            ss << "The attack failed because there was a wall in the way." << std::endl;
            console::slowPrintAndWait(ss.str(), 20, 2200);
            return false;
        } 
    }

    // attack is valid
    float damageDealt = attacker->getStrength() * target->getDefense();
    target->hurt(damageDealt);

    ss.str("");
    ss.clear();

    ss << std::fixed << std::setprecision(2);
    ss << "Player " << attacker->getNameUpper() << " dealt " << damageDealt << " points of damage to player " << target->getNameUpper() << "!";
    console::slowPrintAndWait(ss.str());

    if (!target->isAlive()) {
        ss.str("");
        ss.clear();
        ss << "Player " << target->getNameUpper() << " died!";
        console::slowPrintAndWait(ss.str());
    }

    // if the attacker is an ai, there was already a sleep caused by Game::runGameCycle
    if (attacker->isHuman()) {
        console::waitForMilliseconds();
    }

    return true;
}

void ActionHandler::characterAction(Character *character, QueryOptionsCharacterAction action, std::optional<std::variant<Character*, int>> aiParameter) {
    switch (action) {
    case QueryOptionsCharacterAction::ATTACK: 
        if (character->isHuman()) {
            console::slowPrint("Who do you want to attack?");
            Character *target = console::queryCharacter(characters);
            (void)attemptAttack(character, target);

        } else {
            if (!aiParameter) {
                throw std::runtime_error("Game::aiCharacterAction: missing parameter for action \"ATTACK\"");
            }
            if (!(std::holds_alternative<Character*>(aiParameter.value()))) {
                throw std::runtime_error("Game::aiCharacterAction: received invalid parameter for action \"ATTACK\"");
            }

            (void)attemptAttack(
                character,
                std::get<Character*>(aiParameter.value())
            );
        }
        break;

    case QueryOptionsCharacterAction::STATUS:
        // print some information
        // don't put character to the end of the queue!
        // ^ handled by runGameCycle
        if (!character->isHuman()) {
            throw std::runtime_error("Game::aiCharacterAction: AI character chose STATUS, which should not be possible.");
        }
        character->printStatus();
        console::pressEnterToContinue();
        break;

    case QueryOptionsCharacterAction::PASS:
        // do nothing
        // "pass" ... "i'm not doing anything this round"
        if (!character->isHuman()) {
            throw std::runtime_error("Game::aiCharacterAction: AI character chose PASS, which should not be possible.");
        }
        break;

    case QueryOptionsCharacterAction::USE_ITEM:
        console::slowPrintAndWait("Player " + std::string(1, character->getNameUpper()) + " used an item: " + std::string(itemToString(character->getHeldItem())));
        character->useHeldItem();
        if (character->isHuman()) {
            console::waitForMilliseconds();
        }
        break;

    default: // if it's none of the above, it's probably a direction. if it isn't, let moveCharacter throw an exception
        
        if (character->isHuman()) {
            // query distance
            console::slowPrint("How far do you want to go? Maximum distance: " + std::to_string(character->getSpeed()) + " tiles.");
            int dist = console::readIntInRange(1, character->getSpeed());
            moveCharacter(character, action, dist);
        } else {
            if (!aiParameter) {
                throw std::runtime_error("Game::aiCharacterAction: missing parameter for movement action");
            }

            if (!(std::holds_alternative<int>(aiParameter.value()))) {
                throw std::runtime_error("Game::aiCharacterAction: invalid parameter for movement action");
            }
            
            moveCharacter(character, action, std::get<int>(aiParameter.value()));
        }
        break;
    }
}

void ActionHandler::moveCharacter(Character *character, QueryOptionsCharacterAction direction, int distance) {
    if (distance <= 0 || distance > character->getSpeed()) {
        throw std::invalid_argument(
            "Game::moveCharacter: invalid argument <distance>: " + std::to_string(distance)
            + " is out of range for <character>: " + std::string(1, character->getNameUpper()) 
            + " with maximum <speed>: " + std::to_string(character->getSpeed())
        );
    }
    
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

    std::ostringstream ss;
    ss << "Player " << character->getNameUpper() << " moved to (" << character->getXpos() << ", " << character->getYpos() << ")";
    console::slowPrintAndWait(ss.str());
}