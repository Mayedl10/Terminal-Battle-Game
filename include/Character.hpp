#pragma once

#include "Item.hpp"

enum CharacterClass {
    CC_Invalid,
    CC_Fighter,
    CC_Ranger,
    CC_Mage,
    CC_Rogue
};

class Character {
    ItemType heldItem;
    CharacterClass charClass;
    char name;
    int xpos, ypos;

    int speed;
    int range;
    float strength;
    float defense;  // gets multiplied with damage; range 0.0f~1.0f
    float health;   // range 0.0f~100.0f

public:

    int getSpeed();
    void setSpeed(int newSpeed);

    int getRange();
    void setRange(int newRange);

    float getStrength();
    void setStrength(float newStrength);

    float getDefense();
    void setDefense(float newDefense);
    
    float getHealth();
    void setHealth(float newHealth);

    int getXpos();
    void setXpos(int newXpos);

    int getYpos();
    void setYpos(int newYpos);

    Character(char name, CharacterClass charClass);
};