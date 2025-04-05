CXX = em++
FLAGS = -std=c++20 -g -pthread -Wall -Wextra -DCATCH_CONFIG_MAIN

# Define exported function names here
EXPORTED_FUNCTIONS_LIST = _clearCanvas _addVertex _handleCanvasClick _exportGraph _importGraph

# Citation: Following commands were generated with AI
EXPORTED_FUNCTIONS = $(foreach fn,$(EXPORTED_FUNCTIONS_LIST),'$(fn)',)

.PHONY: build
build:
	$(CXX) $(FLAGS) main.cpp -o main.js -s NO_EXIT_RUNTIME=1 -s EXPORTED_FUNCTIONS="[${EXPORTED_FUNCTIONS} '_main']" -s EXPORTED_RUNTIME_METHODS="['cwrap']"

run: build
	emrun --no_browser --port 8080 .

clean:
	rm -f main.js main.wasm