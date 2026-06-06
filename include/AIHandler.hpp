#pragma once

#include <optional>
#include <variant>
#include <utility>
#include <memory>
#include <deque>

#include "Character.hpp"

class AIHandler {
    std::mt19937& rng;
    std::deque<std::unique_ptr<Character>>& characters;
public:
    std::pair<QueryOptionsCharacterAction, std::optional<std::variant<Character*, int>>> pickActionAI();
    std::pair<QueryOptionsCharacterAction, int> aiPickMovement();

    AIHandler(std::deque<std::unique_ptr<Character>>& characters, std::mt19937& rng) 
    : rng{rng}, characters{characters} {}
};