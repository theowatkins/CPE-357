CC = clang
CFLAGS = -std=c99 -Wall -pedantic -g
LD = clang
LDFLAGS = 

parseline: parseline.o funcs_parse.o
	$(LD) $(LDFLAGS) -o parseline parseline.o funcs_parse.o

parseline.o: parseline.c parseline.h
	$(CC) $(CFLAGS) -c -o parseline.o parseline.c

funcs_parse.o: funcs_parse.c funcs_parse.h
	$(CC) $(CFLAGS) -c -o funcs_parse.o funcs_parse.c
