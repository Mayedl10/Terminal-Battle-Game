#pragma once

#include <vector>
#include <string>
#include <utility>

class InputQuery {
    // pairs: {text, isEnabled}
    std::vector<std::pair<std::string, bool>> options;

public:
    std::vector<std::pair<std::string, bool>>& getOptions();
    int query();
    void addOption(std::pair<std::string, bool>& option);
    void setVisibility(int index, bool isVisible);
    bool isVisible(int index);

    InputQuery(std::vector<std::pair<std::string, bool>>&& opts);
    InputQuery(); // no-arg constructor
};