SHELL = /bin/sh
CFLAGS = -g -Wall
LIBS = -lm
OBJ = compute_mins.o brute_force.o diagonal_approx.o args.o tetboards.o graph.o dynamic_array.o

compute_mins : $(OBJ)
	gcc $(CFLAGS) -o $@ $^ $(LIBS)

%.o : src/%.c
	gcc -c $(CFLAGS) $^

clean :
	rm *.o compute_mins
