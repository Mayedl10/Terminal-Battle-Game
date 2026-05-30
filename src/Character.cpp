#include "Character.hpp"
#include "Item.hpp"
#include "Classes.hpp"
#include "InputQuery.hpp"

#include <iostream>
#include <vector>
#include <cctype>

void Character::setQueryObject(InputQuery& qu) {
    queryObject = qu;
}

// matching InputQuery constructor
void Character::setQueryObject(std::vector<Query>&& opts) {
    queryObject = InputQuery(std::move(opts));  // std::move because of lvalue / rvalue stuff
}

InputQuery& Character::getQueryObject() {
    return queryObject;
}

int Character::getSpeed() {
    return attributes.speed;
}

void Character::setSpeed(int newSpeed) {
    attributes.speed = newSpeed;
    if (attributes.speed < CharacterAttributes::minSpeed) {
        attributes.speed = CharacterAttributes::minSpeed;
    }
    if (attributes.speed > CharacterAttributes::maxSpeed) {
        attributes.speed = CharacterAttributes::maxSpeed;
    }
}

int Character::getRange() {
    return attributes.range;
}

void Character::setRange(int newRange) {
    attributes.range = newRange;
    if (attributes.range < CharacterAttributes::minRange) {
        attributes.range = CharacterAttributes::minRange;
    }
    if (attributes.range > CharacterAttributes::maxRange) {
        attributes.range = CharacterAttributes::maxRange;
    }
}

float Character::getStrength() {
    return attributes.strength;
}

void Character::setStrength(float newStrength) {
    attributes.strength = newStrength;
    if (attributes.strength < CharacterAttributes::minStrength) {
        attributes.strength = CharacterAttributes::minStrength;
    }
    if (attributes.strength > CharacterAttributes::maxStrength) {
        attributes.strength = CharacterAttributes::maxStrength;
    }
}

float Character::getDefense() {
    return attributes.defense;
}

void Character::setDefense(float newDefense) {
    attributes.defense = newDefense;
    // comparison operators are swapped here because min>max (defense is a factor on damage taken)
    if (attributes.defense > CharacterAttributes::minDefense) {
        attributes.defense = CharacterAttributes::minDefense;
    }
    if (attributes.defense < CharacterAttributes::maxDefense) {
        attributes.defense = CharacterAttributes::maxDefense;
    }
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

bool Character::isAlive() {
    return (getHealth() > 0);
}

char Character::getName() {
    if (isHuman()) {
        return name;
    } else {
        // return lowercase name if character isnt human
        return static_cast<char>(std::tolower(name));
    }
}

char Character::getNameUpper() {
    return name;
}

void Character::hurt(float damage) {
    setHealth(getHealth() - damage);
}

void Character::restoreHealth(float amount) {
    setHealth(getHealth() + amount);
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

QueryOptionsCharacterAction Character::pickAction() {
    if (!isHuman()) {
        return pickActionAI();
    }

    return static_cast<QueryOptionsCharacterAction>(queryObject.query());
}

QueryOptionsCharacterAction Character::pickActionAI() {
    return QueryOptionsCharacterAction::PASS; // todo
}

void Character::printStatus() {
    std::cout << "Status for Character " << getNameUpper() << ":\n"
    << "\tHealth: " << getHealth() << "HP\n"
    << "\tStrength: " << getStrength() << "\n"
    << "\tSpeed: " << getSpeed() << "\n"
    << "\tRange: " << getRange() << "\n"
    << "\tPosition: (" << getXpos()+1 << ", " << getYpos()+1 << ")\n"
    << "\tDefense: " << getDefense() << "\n"
    << "\tHeld Item: " << static_cast<int>(heldItem) << " [TODO: implement proper item printing]"
    << std::endl;
}

void Character::initialiseQueryObject() {
    // set default options that every character has
    setQueryObject({
        {"Move N", true, QueryOptionsCharacterAction::MOVE_N},
        {"Move E", true, QueryOptionsCharacterAction::MOVE_E},
        {"Move S", true, QueryOptionsCharacterAction::MOVE_S},
        {"Move W", true, QueryOptionsCharacterAction::MOVE_W},
        {"Attack", true, QueryOptionsCharacterAction::ATTACK},
        {"Get Status", true, QueryOptionsCharacterAction::STATUS},
        {"Do Nothing", true, QueryOptionsCharacterAction::PASS},
        {"Use Item", false, QueryOptionsCharacterAction::USE_ITEM}     // hidden when character doesn't have an item
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