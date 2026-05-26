// this file implements behavious of Character and Game!
// it handles functionalities related to items
// (i didn't want Game.cpp and Character.cpp to become too big)
// (plus, it cant hurt to keep item-related logic close together)
#include "Character.hpp"
#include "Game.hpp"

#include <array>
#include <stdexcept>
#include <iostream>
#include <random>

/*
* note: bounds checking for attributes should be handled by
* the setter methods for Character
*/

void Character::useHeldItem() {
    switch (heldItem) {
        case IT_Invalid:
            throw std::runtime_error("Character::useHeldItem: encountered character with invalid item type: " + getName());
            break;
        case IT_None:
            std::cout << "You cannot use an item unless you are holding one!" << std::endl;
            break;
        case IT_HealthPotion:
            restoreHealth(10.0);
            break;
        case IT_SpeedPotion:
            setSpeed(getSpeed() +2);
            break;
        case IT_TurtlePotion:
            setSpeed(getSpeed() -3);
            setDefense(getDefense() *1.5f);
            break;
        case IT_HealthPotion_II:
            restoreHealth(15.0);
            break;
        case IT_SpeedPotion_II:
            setSpeed(getSpeed() +3);
            break;
        case IT_TurtlePotion_II:
            setSpeed(getSpeed() -4);
            setDefense(getDefense() *1.8f);
            break;
        default:
            // shouldnt happen
            throw std::runtime_error("Character::useHeldItem: encountered character with unknown held item type: " + std::to_string(heldItem) + ". Character: " + getName());
            break;
    }
}

bool Character::attemptPickup(std::unique_ptr<Level>& level) {
    auto& tile = level->getTileAt(getXpos(), getYpos());
    // if ch has no item and floor has an item
    if (heldItem == ItemType::IT_None && tile.item != ItemType::IT_None) {
        heldItem = tile.item;
        tile.item = IT_None;
        return true;
    }
    // if ch already has an item or """floor is empty"""
    return false;
}


ItemType Game::getRandomItemFromWeights() {    
    // static because these thigns aren't gonna change with every function call
    static constexpr std::array<ItemType, 7> items = {
        ItemType::IT_None,
        ItemType::IT_HealthPotion,
        ItemType::IT_HealthPotion_II,
        ItemType::IT_SpeedPotion,
        ItemType::IT_SpeedPotion_II,
        ItemType::IT_TurtlePotion,
        ItemType::IT_TurtlePotion_II
    };
    static constexpr std::array<int, 7> weights = {
        ItemSpawnWeights::ISW_None,
        ItemSpawnWeights::ISW_HealthPotion,
        ItemSpawnWeights::ISW_HealthPotion_II,
        ItemSpawnWeights::ISW_SpeedPotion,
        ItemSpawnWeights::ISW_SpeedPotion_II,
        ItemSpawnWeights::ISW_TurtlePotion,
        ItemSpawnWeights::ISW_TurtlePotion_II
    };

    static std::discrete_distribution<size_t> dist(
        weights.begin(),
        weights.end()
    );

    // rng ... member variable mt
    return items[dist(rng)];
}

void Game::distributeItems() {
    auto& map = getLevel()->getMap();
    // note to self:
    // map is a REFERENCE to the actual level data!
    // changes to said level data are made DIRECTLY!

    for (int y = 0; y < static_cast<int>(map.size()); y++) {
        for (int x = 0; x < static_cast<int>(map[y].size()); x++) {
            if (map[y][x].allowsItemSpawning()) {
                map[y][x].item = getRandomItemFromWeights();
            }
        }
    }
}