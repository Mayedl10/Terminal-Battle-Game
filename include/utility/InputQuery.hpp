#pragma once

#include <vector>
#include <string>
#include <utility>

struct Query {
    std::string message;
    bool isVisible;
    int returnValue;
    Query(std::string msg, bool visible, int value) 
    : message{msg},
      isVisible{visible},
      returnValue{value}
    {}
};

class InputQuery {
    std::vector<Query> options;

public:
    std::vector<Query>& getOptions();
    int query();
    void addOption(Query& option);
    void setVisibility(int index, bool isVisible);
    bool isVisible(int index);

    InputQuery(std::vector<Query>&& opts);
    InputQuery(); // no-arg constructor
};