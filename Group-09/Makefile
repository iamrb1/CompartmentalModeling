COMPILER = emcc
SRC = main.cpp WebLayout/WebLayout.cpp WebLayout/WebLayoutManager.cpp WebLayout/LoadSlideDeck.cpp \
			WebLayout/ExportSlideDeck.cpp TextBox/FormattedText.cpp Image/Image.cpp

WASM_FLAGS = -s WASM=1 -s NO_EXIT_RUNTIME=1
EXPORTED_FUNCS = -s EXPORTED_FUNCTIONS="['_main', '_call_advance', '_call_rewind', '_loadSlideDeckFromJson', '_exportSlideDeckToJson', '_call_addNewSlide', '_pause', '_resume']"
EXPORTED_RUNTIME_METHODS = -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']"

default: presentation

presentation:
	$(MAKE) clean
	$(COMPILER) $(SRC) -std=c++20 -I utils -o output.js $(WASM_FLAGS) $(EXPORTED_FUNCS) $(EXPORTED_RUNTIME_METHODS)
	$(MAKE) server

debug:
	$(MAKE) clean
	$(COMPILER) $(SRC) -std=c++20 -I utils -o output.js $(WASM_FLAGS) $(EXPORTED_FUNCS) $(EXPORTED_RUNTIME_METHODS) -gsource-map -DDEBUG -O0 -sNO_DISABLE_EXCEPTION_CATCHING
	$(MAKE) server

server:
	python3 -m http.server 8000
clean:
	rm -f *.js *.wasm *.map