#include "Character.hpp"
#include "Item.hpp"
#include "Classes.hpp"

#include <cctype>

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
    return this->isHumanPlayer;
}

void Character::setIsHuman(bool isHuman) {
    this->isHumanPlayer = isHuman;
}

char Character::getName() {
    if (this->isHuman()) {
        return this->name;
    } else {
        // return lowercase name if character isnt human
        return (char)(std::tolower(this->name));
    }
}

void Character::setClassAttributes() {
    switch (this->charClass) {
    case CharacterClass::CC_Fighter:
        this->attributes = CharacterClasses::fighterAttr;
        break;
    case CharacterClass::CC_Mage:
        this->attributes = CharacterClasses::mageAttr;
        break;
    case CharacterClass::CC_Rogue:
        this->attributes = CharacterClasses::rogueAttr;
        break;
    case CharacterClass::CC_Ranger:
        this->attributes = CharacterClasses::rangerAttr;
        break;
    default:
        this->attributes = CharacterClasses::invalidAttr;
        break;
    }
}

Character::Character(char name, CharacterClass charClass, bool isHuman) {
    this->name = name;
    this->charClass = charClass;
    setIsHuman(isHuman);

    heldItem = ItemType::IT_None;

    setClassAttributes();
    
    // let Game object set positions
}