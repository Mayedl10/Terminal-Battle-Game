#pragma once

#include <vector>
#include <string>
#include <utility>

class InputQuery {
    // pairs: {text, isEnabled}
    const std::vector<std::pair<std::string, bool>> options;

public:
    const std::vector<std::pair<std::string, bool>>& getOptions() const;
    int query();

    InputQuery(const std::vector<std::pair<std::string, bool>>& opts);
};