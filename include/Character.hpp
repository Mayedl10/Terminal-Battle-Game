#pragma once

#include <array>

#include "Item.hpp"
#include "InputQuery.hpp"

struct CharacterAttributes {
    int speed;
    int range;
    float strength;
    float defense;  // gets multiplied with damage; range 0.0f~1.0f
    float health;   // range 0.0f~100.0f
    bool hasMagic;
};


enum CharacterClass {
    CC_Invalid,
    CC_Fighter,
    CC_Ranger,
    CC_Mage,
    CC_Rogue
};

enum QueryOptionsCharacterAction :int {
    MOVE_N,
    MOVE_S,
    MOVE_E,
    MOVE_W,
    ATTACK,
    STATUS,
    PASS,
    USE_ITEM,
    DROP_ITEM
};

class Character {
    ItemType heldItem;
    CharacterClass charClass;
    char name;
    int xpos, ypos;
    bool isHumanPlayer;

    CharacterAttributes attributes;

    void setClassAttributes();
    InputQuery queryObject;
    
    void setQueryObject(InputQuery& qu);
    void setQueryObject(std::vector<Query>&& opts); // matching InputQuery constructor

    void initialiseQueryObject();

    QueryOptionsCharacterAction pickActionAI();

public:

    static inline const std::array<CharacterClass, 4> validClasses = {
        CharacterClass::CC_Fighter,
        CharacterClass::CC_Mage,
        CharacterClass::CC_Ranger,
        CharacterClass::CC_Rogue
    };

    InputQuery& getQueryObject();

    char getName();
    char getNameUpper();

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

    bool isHuman();
    void setIsHuman(bool isHuman);
    
    bool isAlive();

    void hurt(float damage);

    QueryOptionsCharacterAction pickAction();

    Character(char name, CharacterClass charClass, bool isHuman);
};