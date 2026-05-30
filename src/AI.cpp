#include <random>
#include <array>
#include <stdexcept>
#include <iostream>

#include "Game.hpp"
#include "Character.hpp"
#include "ConsoleHandler.hpp"

// AI is part of game class, because this needs a lot of things from the game object to work
std::pair<QueryOptionsCharacterAction, std::optional<std::variant<Character*, int>>> Game::pickActionAI() {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    auto& current = characters.front();

    // if ai has an item, 50% chance it is used
    if (current->hasItem() && dist(rng) > 0.5f) {
        return {QueryOptionsCharacterAction::USE_ITEM, std::nullopt};
    }

    // is there a character in range?
    auto& closest = getClosestCharacterInRange(current);
    
    // if no, return some movement thing
    if (closest == current) {
        auto movement = aiPickMovement();
        return {
            movement.first,
            movement.second
        }; // return movement; should be fine, but I don't trust implicit type conversions enough
    }
    
    // if yes, return attack
    return {
        QueryOptionsCharacterAction::ATTACK,
        closest.get()
    };
    
    // fallback
    return {QueryOptionsCharacterAction::PASS, std::nullopt};
}

std::pair<QueryOptionsCharacterAction, int> Game::aiPickMovement() {
    auto& current = characters.front();

    // move in a random direction, the AI doesn't have to be very "I", after all
    static constexpr std::array<QueryOptionsCharacterAction, 4> directions = {
        QueryOptionsCharacterAction::MOVE_N,
        QueryOptionsCharacterAction::MOVE_E,
        QueryOptionsCharacterAction::MOVE_S,
        QueryOptionsCharacterAction::MOVE_W
    }; 

    std::uniform_int_distribution<size_t> directionDist(0, directions.size()-1);

    return {
        directions[directionDist(getRNG())],
        current->getSpeed()
    };
}

// this entire function is a MESS because i designed these systems to work for humans first and foremost
void Game::aiCharacterAction(std::unique_ptr<Character>& character, QueryOptionsCharacterAction action, std::optional<std::variant<Character*, int>> aiParameter) {
    switch (action) {
    case QueryOptionsCharacterAction::ATTACK: 
        if (!aiParameter) {
            throw std::runtime_error("Game::aiCharacterAction: missing parameter for action \"ATTACK\"");
        }

        if (!(std::holds_alternative<Character*>(aiParameter.value()))) {
            throw std::runtime_error("Game::aiCharacterAction: received invalid parameter for action \"ATTACK\"");
        }

        
        (void)attemptAttack(
            character.get(),
            std::get<Character*>(aiParameter.value())
        );
    
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
        if (!aiParameter) {
            throw std::runtime_error("Game::aiCharacterAction: missing parameter for movement action");
        }

        if (!(std::holds_alternative<int>(aiParameter.value()))) {
            throw std::runtime_error("Game::aiCharacterAction: invalid parameter for movement action");
        }
        
        moveCharacter(character, action, std::get<int>(aiParameter.value()));
        break;
    }
}