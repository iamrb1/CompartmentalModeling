COMPILER = emcc
SRC = main.cpp WebLayout/WebLayout.cpp TextBox/FormattedText.cpp Image/Image.cpp

WASM_FLAGS = -s WASM=1 -s NO_EXIT_RUNTIME=1
EXPORTED_FUNCS = -s EXPORTED_FUNCTIONS="['_main', '_call_advance', '_call_rewind', '_loadSlideDeckFromJson', '_exportSlideDeckToJson', '_call_addNewSlide', '_call_start', '_call_stop', '_call_isMoveableObject', '_call_addImage', 'lengthBytesUTF8', '_call_addTextBox', '_call_updateTextBoxContent', '_call_updatePosition', '_malloc', '_free', '_call_addSlideChangeEvent', '_call_updateImageSize']"
EXPORTED_RUNTIME_METHODS = -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap', 'stringToUTF8']"

UNAME_S := $(shell uname -s 2>/dev/null)

ifeq ($(OS),Windows_NT)
	IS_WINDOWS := 1
else ifeq ($(UNAME_S),Linux)
	IS_WINDOWS := 0
else ifeq ($(UNAME_S),Darwin)
	IS_WINDOWS := 0
else
	IS_WINDOWS := 1
endif

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
ifeq ($(IS_WINDOWS), 1)
	@python -m http.server 8000 || py -m http.server 8000
else
	@python3 -m http.server 8000 || python -m http.server 8000
endif

clean:
ifeq ($(IS_WINDOWS), 1)
	-del /Q *.js *.wasm *.map *.o 2>nul
else
	rm -f *.js *.wasm *.map *.o
endif
