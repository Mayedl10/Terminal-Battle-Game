#pragma once

#include <utility>
#include <vector>
#include <memory>

struct MathUtils {
    static bool pointInRange(std::pair<int, int> centre, double radius, std::pair<int, int> point);
    static std::vector<std::pair<int, int>> bresenhamsLineAlgorithm(std::pair<int, int> A, std::pair<int, int> B);
    static float pointDistance(std::pair<int, int> A, std::pair<int, int> B);
};