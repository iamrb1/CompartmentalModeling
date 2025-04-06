###############################################################################
# Simple Makefile for building a WebAssembly+SDL2 app via Emscripten
# Adjust sources, flags, etc. as needed.
###############################################################################

# List all your .cpp files here
SOURCES = \
  main.cpp \
  Circle.cpp \
  Surface.cpp \
  Sector.cpp \
  DataTracker.cpp \
  FixedPoint.cpp \
  FunctionSet.cpp

# Automatically convert .cpp file list to .o object list
OBJECTS = $(SOURCES:.cpp=.o)

# The final output name. Emscripten will produce index.html, plus .wasm, .js, etc.
TARGET = index.html

# Emscripten compiler Change the path match your folder
EMCC = ~/emsdk/upstream/emscripten/emcc

# Compilation & Link flags
# -s USE_SDL=2   -> Links against SDL2
# -s WASM=1      -> Generates WebAssembly code
# You can add more flags like optimization (-O2 or -O3), debug info (-g), etc.
CFLAGS = -std=c++17 -s USE_SDL=2 -s WASM=1

###############################################################################
# Build targets
###############################################################################

all: $(TARGET)

# Link step: create index.html from all objects
$(TARGET): $(OBJECTS)
	$(EMCC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

# Compile step: each .cpp -> .o
%.o: %.cpp
	$(EMCC) $(CFLAGS) -c $< -o $@

# Remove generated files
clean:
	rm -f $(OBJECTS) $(TARGET)
