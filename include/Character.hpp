#pragma once

#include "Item.hpp"

enum CharacterClass {
    CC_Invalid
};

class Character {
    ItemType heldItem;
    CharacterClass charClass;
    char name;

    int speed;
    int range;
    float strength;
    float defense;  // gets multiplied with damage; range 0.0f~1.0f
    float health;   // range 0.0f~100.0f

public:

    int getSpeed();
    void setSpeed();

    int getRange();
    void setRange();

    float getStrength();
    void setStrength();

    float getDefense();
    void setDefense();
    
    float getHealth();
    void setHealth();

    Character(char name, CharacterClass charClass);
};