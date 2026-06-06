#include <random>
#include <array>
#include <stdexcept>
#include <iostream>

#include "AIHandler.hpp"
#include "Character.hpp"
#include "MathUtils.hpp"

std::pair<QueryOptionsCharacterAction, std::optional<std::variant<Character*, int>>> AIHandler::pickActionAI() {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    Character *current = characters.front().get();

    // if ai has an item, 50% chance it is used
    if (current->hasItem() && dist(rng) > 0.5f) {
        return {QueryOptionsCharacterAction::USE_ITEM, std::nullopt};
    }

    // is there a character in range?
    Character *closest = math::getClosestCharacterInRange(current, characters);
    
    // if no, return some movement thing
    if (*closest == *current) {
        auto movement = aiPickMovement();
        return {
            movement.first,
            movement.second
        }; // return movement; should be fine, but I don't trust implicit type conversions enough
    }
    
    // if yes, return attack
    return {
        QueryOptionsCharacterAction::ATTACK,
        closest
    };
    
    // fallback
    return {QueryOptionsCharacterAction::PASS, std::nullopt};
}

std::pair<QueryOptionsCharacterAction, int> AIHandler::aiPickMovement() {
    Character *current = characters.front().get();

    // move in a random direction, the AI doesn't have to be very "I", after all
    static constexpr std::array<QueryOptionsCharacterAction, 4> directions = {
        QueryOptionsCharacterAction::MOVE_N,
        QueryOptionsCharacterAction::MOVE_E,
        QueryOptionsCharacterAction::MOVE_S,
        QueryOptionsCharacterAction::MOVE_W
    }; 

    std::uniform_int_distribution<size_t> directionDist(0, directions.size()-1);
    std::uniform_int_distribution<int> speedDist(CharacterAttributes::minSpeed, current->getSpeed());

    return {
        directions[directionDist(rng)],
        speedDist(rng)
    };
}