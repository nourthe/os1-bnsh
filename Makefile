GIT_VERSION := "$(shell git describe --tags --abbrev=0)"

CC=gcc 
CFLAGS=-g -Wall -pedantic -DVERSION=\"$(GIT_VERSION)\"

bnsh: bnsh.o builtin.o parser.o
	$(CC) $(CFLAGS) -o bnsh bnsh.o builtin.o parser.o

bnsh.o: bnsh.c builtin.h parser.h
	$(CC) $(CFLAGS) -c bnsh.c

builtin.o: builtin.c
	$(CC) $(CFLAGS) -c builtin.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

clean:
	rm *.o bnsh
