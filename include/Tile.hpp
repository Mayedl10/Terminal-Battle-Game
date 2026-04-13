#pragma once

#include <cstdint>

#include "Item.hpp"

enum TileType : uint8_t {
    TT_Invalid
};

struct Tile {
    int xpos, ypos;
    TileType type;
    bool blocksProjectiles, blocksMovement;
    ItemType item;  // IT_None if no item is on tile
};