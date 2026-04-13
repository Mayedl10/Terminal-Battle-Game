#pragma once

#include <cstdint>

#include "Character.hpp"

enum ItemType : uint8_t {
    IT_Invalid,
    IT_None,

    IT_HealthPotion,
    IT_EnergyPotion,
    IT_SpeedPotion,
    IT_TurtlePotion,

    // II ... stronger variant
    IT_HealthPotion_II,
    IT_EnergyPotion_II,
    IT_SpeedPotion_II,
    IT_TurtlePotion_II,
};

// the class Item is intended as static and only provides methods
class Item {
public:
    static void useOnCharacter(ItemType type, Character *ch);
};