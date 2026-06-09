#pragma once

#include <string>
#include <optional>

struct GameConfig {
    std::string levelFolderPath;
    int totalPlayers;
    int aiPlayers;
    GameConfig(std::string_view levelFolderPath, int totalPlayers, int aiPlayers)
    : levelFolderPath{levelFolderPath}, totalPlayers{totalPlayers}, aiPlayers{aiPlayers}
    {}
};

namespace titlescreen {
    std::optional<GameConfig> display();
};