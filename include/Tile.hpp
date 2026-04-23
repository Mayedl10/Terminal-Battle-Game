#pragma once

#include <cstdint>

#include "Item.hpp"

enum TileType : uint8_t {
    TT_Invalid,         // ?
    TT_Wall,            // #
    TT_Hole,            // O
    TT_Regular,         // ./! if an item is on this tile
    TT_CharacterSpawn   // @/.
};

struct Tile {
    TileType type;
    bool blocksProjectiles, blocksMovement;
    ItemType item;  // IT_None if no item is on tile
    
    Tile(TileType type, bool blocksProjectiles, bool blocksMovement, ItemType item) :
        type{type},
        blocksProjectiles{blocksProjectiles},
        blocksMovement{blocksMovement},
        item{item}
    {}
};