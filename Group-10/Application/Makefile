OBJS	= main.o
SOURCE	= main.cpp
HEADER	= ../Classes/BruteForceOptimizer.hpp ../Classes/ComboManager.hpp 
OUT	= bellman.out
FLAGS	= -std=c++23 -g3 -c -Wall -Wextra
LFLAGS	= 
CC	= g++

all:	clean	bellman.out

bellman.out: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 

clean:
	rm -f $(OBJS) $(OUT)

valgrind: bellman.out
	valgrind bellman.out

valgrind_leakcheck: bellman.out
	valgrind --leak-check=full bellman.out

valgrind_extreme: bellman.out
	valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --vgdb=yes bellman.out