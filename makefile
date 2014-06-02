# Makefile for HAL9001 main code


CFLAGS=-O2
LFLAGS=
RM=rm -f
INCLUDES=src
OBJS=cubex.o
LIBS=

all: solveV2

solveV1: obj/solveV1.o obj/moves.o
	gcc -g obj/solveV1.o obj/moves.o -o bin/solve_V1

obj/moves.o: src_V1/moves.c src_V1/moves.h src_V1/structures.h
	gcc -c -g src_V1/moves.c -o obj/moves.o

obj/solveV1.o: src_V1/solve.c src_V1/solve.h src_V1/structures.h
	gcc -c -g src_V1/solve.c -o obj/solveV1.o
	
heuristic: obj/moves.o gen_heuristic/obj/gen_heuristic.o
	gcc -g obj/moves.o gen_heuristic/obj/gen_heuristic.o -o gen_heuristic/gen_heuristic
	
gen_heuristic/obj/gen_heuristic.o: gen_heuristic/gen_heuristic.c gen_heuristic/gen_heuristic.h
	gcc -c -g gen_heuristic/gen_heuristic.c -o gen_heuristic/obj/gen_heuristic.o
	
solveV2: src_V2/jaap_sol.c
	gcc -g src_V2/jaap_sol.c -o bin/solve_V2 -Wno-write-strings
    
clean:
	rm -f obj/*
	rm -f bin/*
	rm -f gen_heuristic/obj/*
	rm -f *.exe
	rm -f gen_heuristic/*.exe
	rm -f gen_heuristic/*.stackdump
	rm -f *.stackdump
    


dummy:
