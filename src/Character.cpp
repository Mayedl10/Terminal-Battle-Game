#include "Character.hpp"

#include "Item.hpp"

int Character::getSpeed() {
    return speed;
}

void Character::setSpeed(int newSpeed) {
    speed = newSpeed;
}

int Character::getRange() {
    return range;
}

void Character::setRange(int newRange) {
    range = newRange;
}

float Character::getStrength() {
    return strength;
}

void Character::setStrength(float newStrength) {
    strength = newStrength;
}

float Character::getDefense() {
    return defense;
}

void Character::setDefense(float newDefense) {
    defense = newDefense;
}

float Character::getHealth() {
    return health;
}

void Character::setHealth(float newHealth) {
    health = newHealth;
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

Character::Character(char name, CharacterClass charClass) {
    this->name = name;
    this->charClass = charClass;

    heldItem = ItemType::IT_None;

    // TODO: set more values based on class
    // TODO: initialise members
}