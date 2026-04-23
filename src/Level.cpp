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
                std::cout << '!';
                continue;
            }

            switch (t.type) {
            case (TileType::TT_CharacterSpawn):
                std::cout << '.';
                break;

            case (TileType::TT_Regular):
                std::cout << '.';
                break;

            case (TileType::TT_Hole):
                std::cout << 'O';
                break;

            case (TileType::TT_Wall):
                std::cout << '#';
                break;

            case (TileType::TT_Invalid):
                std::cout << '?';
                break;
            
            default:
                // shouldnt happen
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
        case '@':
            map.back().push_back({TileType::TT_CharacterSpawn, false, false, ItemType::IT_None});
            break;

        // hole
        case 'O':
            map.back().push_back({TileType::TT_Hole, false, true, ItemType::IT_None});
            break;

        // regular floor
        case '.':
            map.back().push_back({TileType::TT_Regular, false, false, ItemType::IT_None});
            break;

        // wall
        case '#':
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