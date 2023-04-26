SHELL=/bin/bash
CC=gcc
CFLAGS=-Wall -ggdb -std=c11
LIBRARY=library
MAIN=main



all: $(LIBRARY).c
	make static
	make shared
	#make dynamic

static: $(LIBRARY).c
	#$(CC) $(CFLAGS) -c $(LIBRARY).c -o $(LIBRARY).o
	#ar crs $(LIBRARY).a $(LIBRARY).o

	$(CC) $(CFLAGS) -c $(MAIN).c -o $(MAIN).o
	ar crs $(MAIN).a $(MAIN).o

shared: $(LIBRARY).c
	$(CC) $(CFLAGS) -fPIC -c $(LIBRARY).c -o $(LIBRARY).o
	$(CC) $(CFLAGS) -shared -o $(LIBRARY).so $(LIBRARY).o

dynamic: $(LIBRARY).c
	$(CC) -c library.c
	$(CC) library.c -o libary -ldl -D DYNAMIC

clean:
	rm -f *.o *.a *.so *.out

