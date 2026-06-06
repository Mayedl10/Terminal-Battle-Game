#include "MathUtils.hpp"
#include "Character.hpp"

#include <vector>
#include <memory>
#include <utility>
#include <cmath>

bool math::pointInRange(std::pair<int, int> centre, double radius, std::pair<int, int> point) {
/*
    // calculate the distance between points using the pythagorean theorem
    double distance = std::sqrt(
        std::pow(point.first - centre.first, 2) +
        std::pow(point.second - centre.second, 2)
    );

    return distance <= radius;
*/

    // optimised version without sqrt - compares squares of numbers directly

    int dx = point.first - centre.first;
    int dy = point.second - centre.second;

    return dx * dx + dy * dy <= radius * radius;
}

// returns a vector of all points on a line between to points using Bresenham's line algorithm
std::vector<std::pair<int, int>> math::bresenhamsLineAlgorithm(std::pair<int, int> A, std::pair<int, int> B) {
    std::vector<std::pair<int, int>> points;

    int x0 = A.first;
    int y0 = A.second;
    int x1 = B.first;
    int y1 = B.second;

    // absolute distances between coordinates
    int dx = std::abs(x0-x1);
    int dy = std::abs(y0-y1);

    // step directions
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx-dy;

    while (true) {
        points.push_back({x0, y0});

        // stop when target is reached
        if (x0 == x1 && y0 == y1) {
            return points;
        }

        int e2 = 2*err;

        // decide which direction to step in (x, y, diagonal)
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

float math::pointDistance(std::pair<int, int> A, std::pair<int, int> B) {
    int dx = B.first - A.first;
    int dy = B.second - A.second;

    return std::sqrt(static_cast<float>(dx * dx + dy * dy));
}

Character* math::getClosestCharacterInRange(Character *character, std::deque<std::unique_ptr<Character>>& characters) {
    float closestDist = MAXFLOAT;
    Character *ret = character;

    for (auto& ch: characters) {
        // pointDistance is used instead of pointInRange, because the distance is needed in multiple checks
        float distance = pointDistance(
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