SHELL = /bin/sh

graphtest : graphtest.o graph.o
	gcc -lm -o graphtest graphtest.o graph.o

graphtest.o graph.o : src/graph/graphtest.c src/graph/graph.c src/graph/graph.h
	gcc -g -c src/graph/graph.c
	gcc -g -c src/graph/graphtest.c
