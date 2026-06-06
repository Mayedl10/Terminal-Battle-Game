#pragma once

#include <string>
#include <fstream>
#include <sstream>

namespace files {

    // can throw exception: std::runtime_error
    std::string read_file(std::string path);

    // can throw exception: std::runtime_error
    void write_file(std::string path, std::string content);

};