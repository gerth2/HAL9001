# Makefile for HAL9001 main code

CC=gcc
LINK=gcc
CFLAGS=-O2
LFLAGS=
RM=rm -f
INCLUDES=src
OBJS=cubex.o
LIBS=

all: build

build: $(CC) src/solve.c -o solve 

clean:
	$(RM) 


dummy:
