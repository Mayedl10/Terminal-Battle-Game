#pragma once

#include <string>
#include <fstream>
#include <sstream>

class FileHandler {
public:

    // can throw exception: std::runtime_error
    static std::string read_file(std::string path) {
        std::ifstream stream(path, std::ios::in | std::ios::binary);
        if (!stream.is_open()) {
            throw std::runtime_error("[[FileHandler]]: Cannot open (rb) file " + path);
        }

        std::ostringstream buffer;
        buffer << stream.rdbuf();

        if (!stream) {
            throw std::runtime_error("[[FileHandler]]: Error reading (rb) file " + path);
        }
        return buffer.str();
    }

    // can throw exception: std::runtime_error
    static void write_file(std::string path, std::string content) {
        std::ofstream stream(path, std::ios::out | std::ios::binary);
        if (!stream.is_open()) {
            throw std::runtime_error("[[FileHandler]]: Cannot open (wb) file " + path);
        }

        stream.write(content.c_str(), content.size());
        if (!stream) {
            throw std::runtime_error("[[FileHandler]]: Error writing (wb) to file " + path);
        }
    }

};