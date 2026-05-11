#pragma once

#include <string>
#include <fstream>
#include <sstream>

class FileHandler {
public:

    // can throw exception: std::runtime_error
    static std::string read_file(std::string path);

    // can throw exception: std::runtime_error
    static void write_file(std::string path, std::string content);

};