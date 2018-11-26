CC=clang
CFLAGS= -std=c99 -Wall -pedantic
LDFLAGS= 
DEBUG= -g

all: parseline

parseline: parseline.o funcs_parse.o
	$(CC) $(DEBUG) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $^

clean: 
	$(RM) parseline  *.o 
