CXX = em++
FLAGS = -std=c++20 -g -pthread -Wall -Wextra -DCATCH_CONFIG_MAIN

# Define exported function names here
EXPORTED_FUNCTIONS_LIST = _drawCircle _clearCanvas _drawGraph _resizeCanvas

# Citation: Following commands were generated with AI
EXPORTED_FUNCTIONS = $(foreach fn,$(EXPORTED_FUNCTIONS_LIST),'$(fn)',)
# EXPORTED_FUNCTIONS := $(EXPORTED_FUNCTIONS:%,%[:-1]) # Remove trailing comma

.PHONY: build
build:
	$(CXX) $(FLAGS) main.cpp -o main.html -s EXPORTED_FUNCTIONS="[${EXPORTED_FUNCTIONS} '_main']" -s EXPORTED_RUNTIME_METHODS="['cwrap']"

run: build
	emrun --no_browser --port 8080 .

clean:
	rm -f main.html main.js main.wasm