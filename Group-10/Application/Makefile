OBJS	= main.o
SOURCE	= main.cpp
HEADER	= ../Classes/BruteForceOptimizer.hpp ../Classes/ComboManager.hpp 
ifeq ($(OS),Windows_NT)
	OUT	= bellman.exe
	CLEAN = del /Q $(OBJS) $(OUT)
else
	OUT = bellman.out
	CLEAN = rm -f $(OBJS) $(OUT)
endif

FLAGS	= -std=c++23 -g3 -c -Wall -Wextra -I../Classes -I../../Group-06/CommandLine/ -I../../Group-06/ArgManager/
LFLAGS	= 
CC	= g++

all:	clean	run

run: $(OUT)
	./$(OUT)

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 

clean:
	$(CLEAN)

valgrind: $(OUT)
	valgrind $(OUT)

valgrind_leakcheck: $(OUT)
	valgrind --leak-check=full $(OUT)

valgrind_extreme: $(OUT)
	valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --vgdb=yes $(OUT)

debug: FLAGS += -DDEBUG
debug: all