##
# Project Title
#
# @file
# @version 0.1

# Compiler
# AI Generated Makefile

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Directories
SRC_DIR = /home/ap/Downloads/CSE498-Spring2025/Group-07
APP_DIR = $(SRC_DIR)/Team07Application
LIB_DIR = $(SRC_DIR)/Team07Library

# Source Files
APP_SRC = $(APP_DIR)/main.cpp
LIB_SRC = $(LIB_DIR)/DataFileManager.cpp

# Output Binary
BIN = main

# Object Files
OBJ = main.o DataFileManager.o

# Default Target
all: $(BIN)

# Compile Source Files into Object Files
main.o: $(APP_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

DataFileManager.o: $(LIB_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link Object Files into Executable
$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Clean Build Files
clean:
	rm -f $(OBJ) $(BIN)

# end
