CC=clang
CFLAGS= -std=c99 -Wall -pedantic
LDFLAGS= 
DEBUG= -g

all: mush

mush: mush.o parseline.o stage_funcs.o errors.o
	$(CC) $(DEBUG) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $^

clean: 
	$(RM) mush  *.o 
