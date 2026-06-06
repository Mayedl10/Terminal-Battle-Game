#include "Level.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <deque>
#include <utility>
#include <algorithm>
#include <unordered_map>

#include "Item.hpp"
#include "Tile.hpp"
#include "Game.hpp"
#include "FileUtils.hpp"
#include "ConsoleUtils.hpp"

std::vector<std::vector<Tile>>& Level::getMap() {
    return map;
}

int Level::getMaxSupportedCharacters() {
    return maxSupportedCharacters;
}

static uint64_t packOccupancyMapIndex(int y, int x) {
// using signed parameters so i can notice bugs where coordinates become negative more easily.
// there is currently no known bug like that, but it can't hurt to have safeguards in place
    if (y < 0 || x < 0) {
        throw std::invalid_argument("packOccupancyMapIndex: cannot pack negative indexes");
    }
    // pack two numbers into the uint64_t used in the hashmap for Level::displayLevel
    return (static_cast<uint64_t>(y) << 32) | x;
}

void Level::displayLevel(std::deque<std::unique_ptr<Character>>& players) {
    std::unordered_map<uint64_t, std::vector<Character*>> occupancyMap;

    // stores pointers to the characters in a way, that they can be acessed via their positions
    for (auto& ch: players) {
        if (!ch->isAlive()) continue; // skip dead players
        
        uint64_t index = packOccupancyMapIndex(
            ch->getYpos(),
            ch->getXpos()
        );
        occupancyMap[index].push_back(ch.get());
        // note: .at() throws an exception when the index doesnt exist, but operator[] creates the element
    }

    /*
    this approach is more efficient than what i originally did.
    originally, i would've created a vector of Character*s on
    every iteration of the inner for-loop. that would've been
    more readable, but this is probably better
    */
    
    for (int y = 0; y < static_cast<int>(map.size()); y++) {
        for (int x = 0; x < static_cast<int>(map[y].size()); x++) {


            // this approach doesn't create every vector every time, like just checking operator[] would
            auto idx = packOccupancyMapIndex(y, x);
            auto it = occupancyMap.find(idx);
            size_t playerCount = 0;
            if (it != occupancyMap.end()) {
                playerCount = it->second.size();
            }

            if (playerCount == 0 && map[y][x].item == ItemType::IT_None) {
                // no characters on this tile, default behaviour

                switch (map[y][x].type) {
                    case TileType::TT_CharacterSpawn:
                        // this tile "looks the same" as TT_Regular
                        std::cout << static_cast<char>(TileType::TT_Regular);
                        break;

                    case TileType::TT_Regular:
                        std::cout << static_cast<char>(map[y][x].type);
                        break;

                    case TileType::TT_Wall:
                        console::printColouredCharBold(
                            static_cast<char>(map[y][x].type),
                            console::style::Magenta
                        );
                        break;

                    case TileType::TT_Hole:
                        console::printColouredCharBold(
                            static_cast<char>(map[y][x].type),
                            console::style::Blue
                        );
                        break;

                    default:
                        throw std::runtime_error("Level::displayLevel: invalid tile type found at x" + std::to_string(x) + " y" + std::to_string(y));
                        break;
                }
            
            } else if (playerCount == 0 && map[y][x].item != ItemType::IT_None) {
                // there is an item here
                console::printColouredCharBold(
                    static_cast<char>(TileType::TT_VISUAL_HasItem),
                    console::style::Green
                );

            } else if (playerCount == 1) {
                // if there is only one character here, print their name
                if (*(players.front().get()) == *(it->second[0])) {
                    console::printColouredCharBold(
                        it->second[0]->getName(),
                        console::style::Yellow
                    );
                } else {
                    console::printColouredChar(
                        it->second[0]->getName(),
                        console::style::Yellow
                    );
                }
            
            } else {
                // if there are multiple, print & to signify that
                console::printColouredChar(
                    static_cast<char>(TileType::TT_VISUAL_Crowded),
                    console::style::Yellow
                );
            }

            std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
}

TileType Level::getTileTypeAt(int x, int y) {
    return getTileAt(x, y).type;
}

Tile& Level::getTileAt(int x, int y) {
    if (x < 0 || y < 0) {
        throw std::invalid_argument("Level::getTileTypeAt: received negative argument(s)");
    }
    if (static_cast<size_t>(y) >= map.size() || static_cast<size_t>(x) >= map[y].size()) {
        throw std::invalid_argument("Level::getTileTypeAt: received out-of-bounds argument(s)");
    }
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
    std::string levelData = files::read_file(levelFilePath);
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
            // you can never know with windows, it likes to add these characters to files
            break;
        
        default:
            map.back().push_back({TileType::TT_Invalid, true, true, ItemType::IT_None});
            break;
        }
    }

    if (map.empty()) {
        throw std::runtime_error("Level::Level: cannot initialise empty level object based on: " + levelFilePath + "\nFile is empty.");
    }

    for (auto& y: map) {
        if (y.empty()) {
            throw std::runtime_error("Level::Level: cannot initialise level object based on " + levelFilePath + "\nMake sure the file contains no extra whitespace (newlines, spaces, tabs, ...).");
        }
    }

}
