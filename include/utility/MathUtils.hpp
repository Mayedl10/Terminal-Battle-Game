#pragma once

#include <utility>
#include <vector>
#include <memory>
#include <deque>

#include "Character.hpp"

namespace math {
    bool pointInRange(std::pair<int, int> centre, double radius, std::pair<int, int> point);
    std::vector<std::pair<int, int>> bresenhamsLineAlgorithm(std::pair<int, int> A, std::pair<int, int> B);
    float pointDistance(std::pair<int, int> A, std::pair<int, int> B);

    // returns reference to the character itself if no other character is in range
    Character* getClosestCharacterInRange(Character *character, std::deque<std::unique_ptr<Character>>& characters);
}