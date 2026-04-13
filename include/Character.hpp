#pragma once

#include "Item.hpp"

enum CharacterClass {
    CC_Invalid
};

class Character {
    ItemType heldItem;
    CharacterClass charClass;
    char name;

public:
    Character(char name, CharacterClass charClass);
};