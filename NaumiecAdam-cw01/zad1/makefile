CC=gcc
CFLAGS=-Wall -ggdb -std=c11
LIBRARY=library



all:
	make static
	make shared

static: $(LIBRARY).c
	$(CC) $(CFLAGS) -c $(LIBRARY).c
	ar crs $(LIBRARY).a $(LIBRARY).o

shared: $(LIBRARY).c
	$(CC) $(CFLAGS) -c -fPIC $(LIBRARY).c
	$(CC) $(CFLAGS) -shared -o $(LIBRARY).so $(LIBRARY).o

clean:
	rm -f *.o *.a *.so
