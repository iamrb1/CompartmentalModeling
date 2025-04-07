COMPILER = emcc
SRC = main.cpp WebLayout/WebLayout.cpp TextBox/FormattedText.cpp Image/Image.cpp

WASM_FLAGS = -s WASM=1 -s NO_EXIT_RUNTIME=1
EXPORTED_FUNCS = -s EXPORTED_FUNCTIONS="['_main', '_call_advance', '_call_rewind', '_loadSlideDeckFromJson', '_exportSlideDeckToJson', '_call_addNewSlide', '_call_start', '_call_stop', '_call_isMoveableObject', '_call_addImage', 'lengthBytesUTF8', '_call_addTextBox', '_call_updateTextBoxContent', '_call_updatePosition', '_malloc', '_free', '_call_addSlideChangeEvent']"
EXPORTED_RUNTIME_METHODS = -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap', 'stringToUTF8']"

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