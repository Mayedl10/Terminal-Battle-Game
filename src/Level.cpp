#include "Level.hpp"

#include <iostream>
#include <vector>
#include <string>

#include "Item.hpp"
#include "Tile.hpp"
#include "Game.hpp"
#include "FileHandler.hpp"

std::vector<std::vector<Tile>>& Level::getMap() {
    return map;
}


// TODO: display players
void Level::displayLevel(Game *game) {
    for (auto row: map) {
        for (Tile& t: row) {
            if (t.item != ItemType::IT_None) {
                std::cout << TileType::TT_VISUAL_HasItem;
                continue;
            }

            switch (t.type) {
            case (TileType::TT_CharacterSpawn):
                std::cout << TileType::TT_Regular;
                break;

            case (TileType::TT_Regular):
                std::cout << TileType::TT_Regular;
                break;

            case (TileType::TT_Hole):
                std::cout << TileType::TT_Hole;
                break;

            case (TileType::TT_Wall):
                std::cout << TileType::TT_Wall;
                break;

            case (TileType::TT_Invalid):
                std::cout << TileType::TT_Invalid;
                break;
            
            default:
                // if an invalid tile is found, print invalid '?' and the actual stored tile type
                // this should not happen anyway, and i should probably throw an exception instead,
                // but this is "good enough" and doesn't cause the program to potentially crash.
                // plus, i will definitely notice if the grid looks off
                std::cout << TileType::TT_Invalid
                << t.type
                << TileType::TT_Invalid;
                break;
            }
            std::cout << ' ';
        }
        std::cout << std::endl;
    }
}

Level::Level(std::string levelFilePath) {
    std::string levelData = FileHandler::read_file(levelFilePath);
    map = {{}};

    for (char c: levelData) {
        switch (c) {
        // player spawnpoint
        case TileType::TT_CharacterSpawn:
            map.back().push_back({TileType::TT_CharacterSpawn, false, false, ItemType::IT_None});
            break;

        // hole
        case TileType::TT_Hole:
            map.back().push_back({TileType::TT_Hole, false, true, ItemType::IT_None});
            break;

        // regular floor
        case TileType::TT_Regular:
            map.back().push_back({TileType::TT_Regular, false, false, ItemType::IT_None});
            break;

        // wall
        case TileType::TT_Wall:
            map.back().push_back({TileType::TT_Wall, true, true, ItemType::IT_None});
            break;

        case '\n':
            map.push_back({}); // next row
            break;

        case '\r':
            // do nothing
            // f*ck you, windows :D
            break;
        
        default:
            map.back().push_back({TileType::TT_Invalid, true, true, ItemType::IT_None});
            break;
        }
    }
}