# Compiler
CXX := g++

# Flags
CXXFLAGS := -std=c++23 -Iinclude -Iinclude/utility -Iutil -Wall -Wextra

# Directories
SRC_DIR := src
BUILD_DIR := build

# Find all .cpp files
SRC_FILES := $(shell find $(SRC_DIR) -name "*.cpp")

# Convert src/foo.cpp -> build/foo.o
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Output binary
TARGET := main

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(TARGET)

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Rebuild
re: clean all

.PHONY: all clean re

# build using: make -j$(nproc)