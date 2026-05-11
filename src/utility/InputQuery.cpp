#include "InputQuery.hpp"
#include "ConsoleHandler.hpp"

#include <string>
#include <vector>
#include <iostream>

std::vector<std::pair<std::string, bool>>& InputQuery::getOptions() {
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

void InputQuery::addOption(std::pair<std::string, bool>& option) {
    options.push_back(option);
}


void InputQuery::setVisibility(int index, bool isVisible) {
    if (index >= options.size() || index < 0) {
        throw std::invalid_argument("InputQuery::setVisibility: index " + std::to_string(index) + " out of range for InputQuery object with " + std::to_string(options.size()) + " available entries");
    }
    options[index].second = isVisible;
}

bool InputQuery::isVisible(int index) {
    if (index >= options.size() || index < 0) {
        throw std::invalid_argument("InputQuery::isVisible: index " + std::to_string(index) + " out of range for InputQuery object with " + std::to_string(options.size()) + " available entries");
    }
    return options[index].second;
}

InputQuery::InputQuery(std::vector<std::pair<std::string, bool>>& opts)
    : options{opts}
    {
        if (options.size() < 1) {
            throw std::invalid_argument("InputQuery::InputQuery: please provide enough options");
        }
    }

InputQuery::InputQuery() 
    : options{{}}
    {
        // no-args constructor doesn't throw exception
    }