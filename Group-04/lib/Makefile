SRC_DIR = ./src
BUILD_DIR = target
OBJ_DIR = artifacts
INC_DIR = ./include
INC_SUB_NAME = cse
INC_SUB_DIR = $(INC_DIR)/$(INC_SUB_NAME)
PROJECT_NAME = cset4

NAMES = $(basename $(notdir $(wildcard $(SRC_DIR)/*.cpp)))
HEADS = $(addprefix $(INC_SUB_DIR)/, $(notdir $(wildcard $(INC_SUB_DIR)/*.hpp)))
SRCS = $(addsuffix .c, $(NAMES))
OBJS = $(addsuffix .o, $(NAMES))

CXX ?= g++

CXX_FLAGS_all ?= -std=c++23 -Wextra -Wall -fpic -I$(INC_DIR)
CXX_FLAGS ?= -O2 -DNDEBUG $(CXX_FLAGS_all)
CXX_FLAGS_debug ?= -ggdb $(CXX_FLAGS_all)

build: BUILD_DIR := $(BUILD_DIR)/release
build: $(PROJECT_NAME)

debug: CXX_FLAGS := $(CXX_FLAGS_debug)
debug: BUILD_DIR := $(BUILD_DIR)/debug
debug: $(PROJECT_NAME)

debug testbin: CXX_FLAGS := $(CXX_FLAGS_debug)
debug testbin: BUILD_DIR := $(BUILD_DIR)/debug
debug testbin: $(PROJECT_NAME)

.PHONY: build_dir

$(PROJECT_NAME): build_dir $(OBJS)
	$(CXX) $(CXX_FLAGS) -shared -o $(BUILD_DIR)/lib$(PROJECT_NAME).so $(addprefix $(BUILD_DIR)/$(OBJ_DIR)/, $(OBJS))

testbin:
	$(CXX) $(CXX_FLAGS) -L$(BUILD_DIR) -l$(PROJECT_NAME) -o $(BUILD_DIR)/testbin main.cpp

clean:
	rm -rf $(BUILD_DIR)

install: build
	install -m 755 $(BUILD_DIR)/lib$(PROJECT_NAME).so /usr/lib/lib$(PROJECT_NAME).so
	cp -rf $(INC_SUB_DIR) /usr/include

remove:
	rm -rf /usr/include/$(INC_SUB_NAME)
	rm -rf /usr/lib/lib$(PROJECT_NAME).so

%.o: $(SRC_DIR)/%.cpp $(HEADS)
	$(CXX) $(CXX_FLAGS) -c -o $(BUILD_DIR)/$(OBJ_DIR)/$@ $<

build_dir:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(OBJ_DIR)
