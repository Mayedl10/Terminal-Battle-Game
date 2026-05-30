#pragma once

#include <array>
#include <memory>
#include <random>

#include "Item.hpp"
#include "InputQuery.hpp"
#include "Level.hpp"

struct CharacterAttributes {

    // upper and lower bounds for those values that need bounds checking
    static inline const int minSpeed = 1;
    static inline const int maxSpeed = 20;
    static inline const int minRange = 2;
    static inline const int maxRange = 20;
    static inline const float minStrength = 1;
    static inline const float maxStrength = 20;
    static inline const float minDefense = 1.0f;     // min > max because it's a factor on damage
    static inline const float maxDefense = 0.1f;


    int speed;
    int range;
    float strength;
    float defense;  // gets multiplied with damage; range 0.0f~1.0f
    float health;   // range 0.0f~100.0f
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
    USE_ITEM
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

public:

    static inline const std::array<CharacterClass, 4> validClasses = {
        CharacterClass::CC_Fighter,
        CharacterClass::CC_Mage,
        CharacterClass::CC_Ranger,
        CharacterClass::CC_Rogue
    };

    InputQuery& getQueryObject();

    char getName() const;
    char getNameUpper() const;

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

    bool isHuman() const;
    void setIsHuman(bool isHuman);
    
    bool isAlive();
    bool hasItem();
    
    void hurt(float damage);
    void restoreHealth(float amount);
    void printStatus();
    QueryOptionsCharacterAction pickAction();
    
    void useHeldItem();
    bool attemptPickup(std::unique_ptr<Level>& level);

    bool operator==(const Character& other) const {
        return other.getNameUpper() == getNameUpper();
    }

    Character(char name, CharacterClass charClass, bool isHuman);
};