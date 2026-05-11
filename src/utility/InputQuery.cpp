#include "InputQuery.hpp"
#include "ConsoleHandler.hpp"

#include <string>
#include <vector>
#include <iostream>

const std::vector<std::pair<std::string, bool>>& InputQuery::getOptions() const {
    return options;
}

int InputQuery::query() {

    int enabledCount = 0;
    for (auto& option: getOptions()) {
        if (option.second) {
            enabledCount++;
        }
    }

    if (enabledCount < 1) {
        throw std::runtime_error("InputQuery::query: cannot produce valid query without any enabled options");
    }

    std::cout << "Please select one of the followind options" << std::endl;
    for (int i = 0; i < options.size(); i++) {
        if (options[i].second) // skip disabled options
            std::cout << "[" << i+1 << "] " << options[i].first << std::endl;
    }
    
    // ConsoleHandler::readIntInRange()

}

InputQuery::InputQuery(const std::vector<std::pair<std::string, bool>>& opts)
    : options{opts}
    {
        if (options.size() < 1) {
            throw std::invalid_argument("InputQuery::InputQuery: please provide enough options");
        }
    }