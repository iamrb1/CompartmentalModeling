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

OBJECTS = $(SOURCES:.cpp=.o)
TARGET = index.html

EMCC = ~/emsdk/upstream/emscripten/emcc

CFLAGS = -std=c++20 -s USE_SDL=2 -s WASM=1 -O2 -g

###############################################################################

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(EMCC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(EMCC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

serve:
	emrun --no_browser --port 8080 .
