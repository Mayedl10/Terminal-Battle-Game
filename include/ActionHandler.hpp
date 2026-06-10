#pragma once

#include "Character.hpp"
#include "Level.hpp"

#include <variant>
#include <optional>

class ActionHandler {
    std::deque<std::unique_ptr<Character>>& characters;
    Level *level;
public:

    // returns false if the attack failed
    // if it succeeds, the function applies damage automatically
    bool attemptAttack(Character *attacker, Character *target, bool actsAsHuman);
    void characterAction(Character *character, QueryOptionsCharacterAction action, std::optional<std::variant<Character*, int>> aiParameter, bool actsAsHuman);
    void moveCharacter(Character *character, QueryOptionsCharacterAction direction, int distance);

    ActionHandler(std::deque<std::unique_ptr<Character>>& characters, Level *level)
    : characters{characters}, level{level} {}
};