#include "Level.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <deque>
#include <utility>
#include <algorithm>

#include "Item.hpp"
#include "Tile.hpp"
#include "Game.hpp"
#include "FileHandler.hpp"

std::vector<std::vector<Tile>>& Level::getMap() {
    return map;
}

int Level::getMaxSupportedCharacters() {
    return maxSupportedCharacters;
}

void Level::displayLevel(std::deque<std::unique_ptr<Character>>& players) {

    std::vector<std::pair<int, int>> playerPositions;


    for (int i = 0; i < static_cast<int>(map.size()); i++) {
        auto row = map[i];
        for (int j = 0; j < static_cast<int>(row.size()); j++) {
            auto& t = row[j];

            int playerIdx = -1;
            for (int k = 0; k < static_cast<int>(players.size()); k++) {
                if (players[k]->getXpos() == j && players[k]->getYpos() == i) {
                    playerIdx = k;
                }
            }

            if (playerIdx >= 0) {
                // there is a player on this field
                std::cout << players[playerIdx]->getName();

            } else if (t.item != ItemType::IT_None) {
                // there is an item on this field
                std::cout << TileType::TT_VISUAL_HasItem;
            
            } else {

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
            }
            std::cout << ' ';
        }
        std::cout << std::endl;
    }
}

TileType Level::getTileTypeAt(int x, int y) {
    return map[y][x].type;
}

Tile& Level::getTileAt(int x, int y) {
    return map[y][x];
}

int Level::getWidth() {
    if (map.size() == 0) {
        return 0;
    }
    return static_cast<int>(map[0].size());
}

int Level::getHeight() {
    if (map.size() == 0) {
        return 0;
    }
    return static_cast<int>(map.size());
}

Level::Level(std::string levelFilePath) {
    std::string levelData = FileHandler::read_file(levelFilePath);
    map = {{}};
    maxSupportedCharacters = 0;

    for (char c: levelData) {
        switch (c) {
        // player spawnpoint
        case TileType::TT_CharacterSpawn:
            maxSupportedCharacters++;
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