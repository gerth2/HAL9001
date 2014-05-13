# Makefile for HAL9001 main code


CFLAGS=-O2
LFLAGS=
RM=rm -f
INCLUDES=src
OBJS=cubex.o
LIBS=

all: solve

solve: obj/solve.o obj/moves.o
	gcc obj/solve.o obj/moves.o -o solve

obj/moves.o: src/moves.c src/moves.h src/structures.h
	gcc -c src/moves.c -o obj/moves.o

obj/solve.o: src/solve.c src/solve.h src/structures.h
	gcc -c src/solve.c -o obj/solve.o

clean:
	rm -f obj/*


dummy:
