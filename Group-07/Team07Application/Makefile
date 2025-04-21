# Anand
# AI generated makefile

# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20

# Directories
SRC_DIR = ..
LIB_DIR = $(SRC_DIR)/Team07Library

# Source Files
SRC = main.cpp
LIB_SRC = $(LIB_DIR)/DataFileManager.cpp

# Include Directories
INCLUDES = -I$(LIB_DIR)

# Output Binary
BIN = sim

# Libraries
LIBS = -lboost_system -lboost_filesystem

# Default Target
all: $(BIN)

# Link Object Files into Executable
$(BIN): $(SRC) $(LIB_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ $(LIBS) -o $@

# Run the Program
run: $(BIN)
	./$(BIN)

# Clean Build Files
clean:
	rm -f $(BIN)
