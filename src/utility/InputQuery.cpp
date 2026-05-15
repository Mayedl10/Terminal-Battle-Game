#include "InputQuery.hpp"
#include "ConsoleHandler.hpp"

#include <string>
#include <vector>
#include <iostream>

std::vector<Query>& InputQuery::getOptions() {
    return options;
}

int InputQuery::query() {

    int enabledCount = 0;
    for (auto& option: getOptions()) {
        if (option.isVisible) {
            enabledCount++;
        }
    }

    if (enabledCount < 1) {
        throw std::runtime_error("InputQuery::query: cannot produce valid query without any enabled options");
    }

    std::cout << "Please select one of the following options" << std::endl;
    for (int i = 0; i < static_cast<int>(options.size()); i++) {
        if (options[i].isVisible) // skip disabled options
            std::cout << "[" << i+1 << "] " << options[i].message << std::endl;
    }

    int idx;

    do {

        // ConsoleHandler::readIntInRange()
        idx = ConsoleHandler::readIntInRange(1, options.size());

        if (!options[idx-1].isVisible) {
            std::cout << "Please select a valid option. Some options have been disabled." << std::endl;
        }

        // handle disabled options
    } while (!options[idx-1].isVisible);


    return options[idx-1].returnValue;
}

void InputQuery::addOption(Query& option) {
    options.push_back(option);
}


void InputQuery::setVisibility(int index, bool isVisible) {
    if (index >= static_cast<int>(options.size()) || index < 0) {
        throw std::invalid_argument("InputQuery::setVisibility: index " + std::to_string(index) + " out of range for InputQuery object with " + std::to_string(options.size()) + " available entries");
    }
    options[index].isVisible = isVisible;
}

bool InputQuery::isVisible(int index) {
    if (index >= static_cast<int>(options.size()) || index < 0) {
        throw std::invalid_argument("InputQuery::isVisible: index " + std::to_string(index) + " out of range for InputQuery object with " + std::to_string(options.size()) + " available entries");
    }
    return options[index].isVisible;
}

InputQuery::InputQuery(std::vector<Query>&& opts)
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