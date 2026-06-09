#include <string>
#include <optional>
#include <sstream>
#include <iostream>

#include "TitleScreen.hpp"
#include "InputQuery.hpp"
#include "ConsoleUtils.hpp"
#include "Level.hpp"

namespace {
void help() {
    console::clearScreen();
    std::ostringstream ss;

    ss
    << "\nRules:"
        << "\n\tAt the beginning of the game, every (non-AI) player selects their class. Classes vary in their strengths and weaknesses. Every turn, a player gets to take an action."
    << "\nClasses:"
        << "\n\t- Fighter: Fighters are can attack targets very close to them. They are moderately strong and fast. They are the jack of all trades."
        << "\n\t- Sorcerer: Sorcerers can harness magic to deal high damage over relatively long distances, but they are not very durable, as their defense is very low."
        << "\n\t- Rogue: Rogues move quickly across the battlefield. They can attack targets relatively close to them, and they have a high defense."
        << "\n\t- Ranger: Rangers are very slow, but they can attack targets from very far away."
    << "\nActions:"
        << "\n\t- Move: You can move in any of the four cardinal directions, but you cannot get past walls or holes in the ground."
        << "\n\t- Attack: You can attempt to attack a player. This fails if there is a wall in the way, or your chosen target is too far away."
        << "\n\t- Status: This does not use up your turn. You can use this action to get some information about your character (stats, held item, etc)."
        << "\n\t- Do Nothing: You can waste your turn doing absolutely nothing."
        << "\n\t- Use Item: If you have an item, this option will appear."
    << "\nUI: The UI is split into a map, and the character list. The character list displays the turn order of all living players, as well as their health. The symbols on the map have different meanings:"
        << "\n\t- " << TileType::TT_Regular << " is regular ground you can walk on."
        << "\n\t- " << TileType::TT_Wall << " is a wall. They block movement and attacks."
        << "\n\t- " << TileType::TT_Hole << " is a hole in the ground. They block movement, but don't block attacks."
        << "\n\t- " << TileType::TT_VISUAL_HasItem << " is a regular floor tile with an item on it. Step onto the tile to pick up the item (unless you already have one)."
        << "\n\t- " << TileType::TT_VISUAL_Crowded << " means, that there are two or more players occupying the same space."
        << "\n\t- Letters represent characters. Lowercase letters are AIs.";

    console::slowPrint(ss.str(), 2);
    console::pressEnterToContinue();

}

GameConfig grabGameConfiguration() {
    console::slowPrint("How many players are there in total?");
    int totalPlayers = console::readIntInRange(2, 26);
    console::slowPrint("How many of those should be controlled by an AI?");
    int aiPlayerCount = console::readIntInRange(0, totalPlayers-1);

    return {
        defaultLevelFilePath,
        totalPlayers,
        aiPlayerCount
    };
}
}

std::optional<GameConfig> titlescreen::display() {

    console::clearScreen();
    std::cout << console::style::Bold << ">> TERMINAL BATTLE GAME <<" << console::style::Reset
    << "\nhttps://github.com/Mayedl10/Terminal-Battle-Game/" << std::endl;
    console::waitForMilliseconds(2000);
    std::cout << std::endl;
    console::pressEnterToContinue();

    enum TitleScreenOptions :int {
        Start,
        Help,
        Quit
    };
    
    InputQuery query{ {
        {"Start", true, TitleScreenOptions::Start},
        {"Help", true, TitleScreenOptions::Help},
        {"Quit", true, TitleScreenOptions::Quit}
    } };
    
    while (true) {
        console::clearScreen();
        TitleScreenOptions response = static_cast<TitleScreenOptions>(query.query());

        if (response == TitleScreenOptions::Quit) {
            return std::nullopt;
        
        } else if (response == TitleScreenOptions::Help) {
            help();
        
        } else {
            return {grabGameConfiguration()};
        }
    }

}
