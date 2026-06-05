#pragma once

#include "Item.hpp"

// enum has two uses:
// - a Tile stores its own type using these
// - this enum stores what tile is associated with which symbol (with exceptions) 
enum TileType : char {
    TT_Invalid          = '?',
    TT_Wall             = '#',
    TT_Hole             = '*',
    TT_Regular          = '.',
    TT_CharacterSpawn   = '@',
    TT_VISUAL_HasItem   = '!',   // no tile should actually have this
    TT_VISUAL_Crowded   = '&'    // ^. used when multiple players are on the same tile
};

struct Tile {
    TileType type;
    bool blocksProjectiles, blocksMovement;
    ItemType item;  // IT_None if no item is on tile

    bool allowsItemSpawning() {
        // block item spawning if item is already there
        if (item != ItemType::IT_None) {
            return false;
        }
        // only allow spawning on regular terrain
        if (type == TileType::TT_Regular) {
            return true;
        }
        return false;
    }
    
    Tile(TileType type, bool blocksProjectiles, bool blocksMovement, ItemType item) :
        type{type},
        blocksProjectiles{blocksProjectiles},
        blocksMovement{blocksMovement},
        item{item}
    {}
};