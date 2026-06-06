#pragma once

#include <utility>
#include <vector>
#include <memory>

namespace math {
    bool pointInRange(std::pair<int, int> centre, double radius, std::pair<int, int> point);
    std::vector<std::pair<int, int>> bresenhamsLineAlgorithm(std::pair<int, int> A, std::pair<int, int> B);
    float pointDistance(std::pair<int, int> A, std::pair<int, int> B);
};