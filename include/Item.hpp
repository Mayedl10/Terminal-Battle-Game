#pragma once

#include <cstdint>

enum ItemType : uint8_t {
    IT_Invalid,
    IT_None,

    IT_HealthPotion,
    IT_SpeedPotion,
    IT_TurtlePotion,

    // II ... stronger variant
    IT_HealthPotion_II,
    IT_SpeedPotion_II,
    IT_TurtlePotion_II,
};

// the weights used for spawning items
// with these weights, a 50x50 field will have 211 items
enum ItemSpawnWeights : int {
    ISW_None            = 2289,
    ISW_HealthPotion    = 90,
    ISW_SpeedPotion     = 30,
    ISW_TurtlePotion    = 30,
    ISW_HealthPotion_II = 45,
    ISW_SpeedPotion_II  = 8,
    ISW_TurtlePotion_II = 8,
};

constexpr std::string_view itemToString(ItemType type) {
    switch (type) {
        case IT_Invalid:            return "[INVALID]";
        case IT_None:               return "None";
        case IT_HealthPotion:       return "Health Potion";
        case IT_SpeedPotion:        return "Speed Potion";
        case IT_TurtlePotion:       return "Turtle Potion";
        case IT_HealthPotion_II:    return "Strong Health Potion";
        case IT_SpeedPotion_II:     return "Strong Speed Potion";
        case IT_TurtlePotion_II:    return "Strong Turtle Potion";
        default:                    return "[UNKNOWN]";
    }
}