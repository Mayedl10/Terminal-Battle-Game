# build using: make -j$(nproc)
# makefile generated using ChatGPT

# Compiler
CXX := g++

# Flags
CXXFLAGS := -std=c++23 -Iinclude -Iinclude/utility -Iutil -Wall -Wextra -MMD -MP -O2 -static-libstdc++ -static-libgcc

# Directories
SRC_DIR := src
BUILD_DIR := build

# Find all .cpp files
SRC_FILES := $(shell find $(SRC_DIR) -name "*.cpp")

# Convert src/foo.cpp -> build/foo.o
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Dependency files
DEP_FILES := $(OBJ_FILES:.o=.d)

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

# Include dependency files
-include $(DEP_FILES)

# Clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Rebuild
re: clean all

.PHONY: all clean re