#include <vector>
#include <string>
#include <deque>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <utility>
#include <cmath>
#include <optional>
#include <variant>
#include <sstream>
#include <iomanip>

#include "Game.hpp"
#include "Character.hpp"
#include "Level.hpp"
#include "Classes.hpp"
#include "Tile.hpp"
#include "InputQuery.hpp"
#include "MathUtils.hpp"
#include "ConsoleUtils.hpp"


// returns false if the game is over
bool Game::runGameCycle() {
    Character *current = characters.front().get();

    console::clearScreen();
    int aliveCount = 0;
    for (auto& ch: characters) {
        if (ch->getHealth() > 0.0f) {
            aliveCount++;
        }
    }
    // exit if the game is over
    if (aliveCount < 2) {
        return false;
    }

    if (!(current->isAlive())) {
        enqueueFrontCharacter();
        return true;
    }

    getLevel()->displayLevel(characters);
    std::cout << "It's player " << current->getNameUpper() << "'s turn!" << std::endl;
    for (auto& p: characters) {
        // print character HP
        if (!p->isAlive()) continue; // skip dead players
        std::cout << "\t" << p->getNameUpper() << ": ";
        console::printHealthBar(p->getHealth());
        std::cout << " " << p->getHealth() << "HP" << std::endl;
    }

    // hide/show "use item" actions
    current->getQueryObject().setVisibility(QueryOptionsCharacterAction::USE_ITEM, current->hasItem());

    // the following two if statements could be merged into one, but the
    // current structure makes the control flow easier to understand

    // query front-of-queue character for an action or make AI decide
    QueryOptionsCharacterAction choice;
    std::optional<std::variant<Character*, int>> AIParameter = std::nullopt;
    
    if (current->isHuman()) {
        choice = current->pickAction();
    } else {
        auto aiChoice = aiHandler->pickActionAI();
        choice = aiChoice.first;
        AIParameter = aiChoice.second;
    }

    // execute actions
    actionHandler->characterAction(current, choice, AIParameter);
    if (!current->isHuman()) {
        console::waitForMilliseconds();
    }

    // pick up items
    for (auto& ch: characters) {
        // the variable "pickup" is only used to make the code more "readable"
        bool pickup = ch->attemptPickup(getLevel());
        if (pickup) {
            console::slowPrintAndWait("Player " + std::string(1, ch->getNameUpper()) + " picked up an item!");
            console::waitForMilliseconds();
        }
    }

    // move front-of-queue character to the back
    // let character move again if status was queried
    if (choice != QueryOptionsCharacterAction::STATUS) {
        enqueueFrontCharacter();
    }

    // no need to check if the game is over, the next cycle deals with that
    return true;
}

Game::Game(const std::string& levelFolderPath, const int characterCount, const int AIcharacterCount)
    : rng{std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count())}
    {
    console::clearScreen();
    // seed random number engine with unix epoch

    loadLevels(levelFolderPath);
    loadPlayers(characterCount, AIcharacterCount);

    selectRandomLevel();
    placePlayers();
    distributeItems();  // randomly spawn items on regular floor tiles 

    actionHandler = std::make_unique<ActionHandler>(characters, getLevel());
    aiHandler = std::make_unique<AIHandler>(characters, rng);

    /*
    * sort characters by speed
    * 
    * an std::deque is used, so i can a) sort it and b) treat it like a regular queue
    * this way, I can always handle only the character at the front for each turn, without
    * having to worry about weird indexing shenanigans
    */
    std::sort(characters.begin(), characters.end(), 
        [](const std::unique_ptr<Character>& a, const std::unique_ptr<Character>& b) {
            return (a->getSpeed() > b->getSpeed()); // high speed comes first
        }
    );

}