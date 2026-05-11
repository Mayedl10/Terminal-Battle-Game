#include "Character.hpp"
#include "Item.hpp"
#include "Classes.hpp"
#include "InputQuery.hpp"

#include <cctype>

void Character::setQueryObject(InputQuery& qu) {
    queryObject = qu;
}

// matching InputQuery constructor
void Character::setQueryObject(std::vector<std::pair<std::string, bool>>&& opts) {
    queryObject = InputQuery(opts);
}

InputQuery& Character::getQueryObject() {
    return queryObject;
}

int Character::getSpeed() {
    return attributes.speed;
}

void Character::setSpeed(int newSpeed) {
    attributes.speed = newSpeed;
}

int Character::getRange() {
    return attributes.range;
}

void Character::setRange(int newRange) {
    attributes.range = newRange;
}

float Character::getStrength() {
    return attributes.strength;
}

void Character::setStrength(float newStrength) {
    attributes.strength = newStrength;
}

float Character::getDefense() {
    return attributes.defense;
}

void Character::setDefense(float newDefense) {
    attributes.defense = newDefense;
}

float Character::getHealth() {
    return attributes.health;
}

void Character::setHealth(float newHealth) {
    attributes.health = newHealth;
}

int Character::getXpos() {
    return xpos;
}

void Character::setXpos(int newXpos) {
    xpos = newXpos;
}

int Character::getYpos() {
    return ypos;
}

void Character::setYpos(int newYpos) {
    ypos = newYpos;
}

bool Character::isHuman() {
    return isHumanPlayer;
}

void Character::setIsHuman(bool isHuman) {
    isHumanPlayer = isHuman;
}

char Character::getName() {
    if (isHuman()) {
        return name;
    } else {
        // return lowercase name if character isnt human
        return (char)(std::tolower(name));
    }
}

char Character::getNameUpper() {
    return name;
}

void Character::setClassAttributes() {
    switch (charClass) {
    case CharacterClass::CC_Fighter:
        attributes = CharacterClasses::fighterAttr;
        break;
    case CharacterClass::CC_Mage:
        attributes = CharacterClasses::mageAttr;
        break;
    case CharacterClass::CC_Rogue:
        attributes = CharacterClasses::rogueAttr;
        break;
    case CharacterClass::CC_Ranger:
        attributes = CharacterClasses::rangerAttr;
        break;
    default:
        attributes = CharacterClasses::invalidAttr;
        break;
    }
}

void Character::initialiseQueryObject() {
    // set default options that every character has
    setQueryObject({
        {"Move N", true},
        {"Move E", true},
        {"Move S", true},
        {"Move W", true},
        {"Attack", true},
        {"Get Status", true},
        {"Do Nothing", true},
        {"Use Item", false},    // hidden when character doesn't have an item
        {"Drop Item", false}    // ^
    });
}

Character::Character(char name, CharacterClass charClass, bool isHuman) {
    this->name = name;
    this->charClass = charClass;
    setIsHuman(isHuman);

    heldItem = ItemType::IT_None;

    setClassAttributes();
    initialiseQueryObject();
    
    // let Game object set positions
}