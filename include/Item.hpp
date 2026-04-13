#pragma once

#include <cstdint>

#include "Character.hpp"

enum ItemType : uint8_t {
    IT_Invalid,
    IT_None
};

class Item {
public:
    static void useOnCharacter(ItemType type, Character *ch);
};