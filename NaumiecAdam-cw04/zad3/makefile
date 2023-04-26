CC=gcc
CFLAGS=-Wall -ggdb -std=c11



all:
	make sender
	make catcher
	make clean

sender: sender.c
	$(CC) $(CFLAGS) -c sender.c
	$(CC) $(CFLAGS) -o sender sender.o

catcher: catcher.c
	$(CC) $(CFLAGS) -c catcher.c
	$(CC) $(CFLAGS) -o catcher catcher.o

clean:
	rm -f $(TASK) *.o *.exe *.out *.a *.so
