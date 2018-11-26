CC=clang
CFLAGS= -std=c99 -Wall -pedantic
LDFLAGS= 
DEBUG= -g

all: parseline

parseline: parseline.o stage_funcs.o errors.o
	$(CC) $(DEBUG) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $^

clean: 
	$(RM) parseline  *.o 
