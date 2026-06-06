#include <deque>
#include <memory>
#include <random>
#include <float.h>
#include <stdexcept>

#include "Game.hpp"
#include "MathUtils.hpp"
#include "Character.hpp"

// this file holds implementations of Game members that could be seen as "utility"

// move a character to the back of the queue
void Game::enqueueFrontCharacter() {
    if (characters.empty()) {
        throw std::runtime_error("Game::enqueueFrontCharacter: cannot enqueue front of an empty std::deque");
    }

    auto front = std::move(characters.front());
    characters.pop_front();
    characters.emplace_back(std::move(front));
}

Level* Game::getLevel() {
    return levels[getLevelIdx()].get();
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

Character* Game::getClosestCharacterInRange(Character *character) {
    float closestDist = MAXFLOAT;
    Character *ret = character;

    for (auto& ch: characters) {
        // pointDistance is used instead of pointInRange, because the distance is needed in multiple checks
        float distance = math::pointDistance(
            {ch->getXpos(), ch->getYpos()},
            {character->getXpos(), character->getYpos()}
        );
        if (
            !(*ch == *character) // overloaded == on Character
            &&
            distance <= static_cast<float>(character->getRange())
            &&
            distance < closestDist
            &&
            ch->isAlive() // cant select dead players
        ) {
            closestDist = distance;
            ret = ch.get();
        }
    }

    return ret;
}
