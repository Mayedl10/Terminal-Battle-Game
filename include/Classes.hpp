#pragma once

#include <vector>
#include <string>
#include <utility>

#include "Character.hpp"

// todo: balancing

struct CharacterClasses {

    inline static const CharacterAttributes fighterAttr {
        // int speed;
        4,
        // int range;
        1,
        // float strength;
        5.0f,
        // float defense;  // gets multiplied with damage; range 0.0f~1.0f
        0.4f,
        // float health;   // range 0.0f~100.0f
        50.0f,
        // bool hasMagic;
        false
    };

    inline static const CharacterAttributes rangerAttr {
        // int speed;
        2,
        // int range;
        10,
        // float strength;
        2.0f,
        // float defense;  // gets multiplied with damage; range 0.0f~1.0f
        0.6f,
        // float health;   // range 0.0f~100.0f
        40.0f,
        // bool hasMagic;
        false
    };

    inline static const CharacterAttributes mageAttr {
        // int speed;
        3,
        // int range;
        8,
        // float strength;
        10.0f,
        // float defense;  // gets multiplied with damage; range 0.0f~1.0f
        1.0f,
        // float health;   // range 0.0f~100.0f
        30.0f,
        // bool hasMagic;
        true
    };

    inline static const CharacterAttributes rogueAttr {
        // int speed;
        5,
        // int range;
        2,
        // float strength;
        4.0f,
        // float defense;  // gets multiplied with damage; range 0.0f~1.0f
        0.4f,
        // float health;   // range 0.0f~100.0f
        40.0f,
        // bool hasMagic;
        false
    };

    inline static const CharacterAttributes invalidAttr {
        // int speed;
        0,
        // int range;
        0,
        // float strength;
        0.0f,
        // float defense;  // gets multiplied with damage; range 0.0f~1.0f
        0.0f,
        // float health;   // range 0.0f~100.0f
        0.0f,
        // bool hasMagic;
        false
    };

};