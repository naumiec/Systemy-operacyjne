CC=gcc
CFLAGS=-Wall -ggdb -std=c11
TASK=files_sizes

all:
	make $(TASK)
	make clean

$(TASK): files_sizes.c
	$(CC) $(CFLAGS) -c $(TASK).c
	$(CC) $(CFLAGS) -o $(TASK) $(TASK).o

clean:
	rm -f $(TASK) *.o *.exe *.out *.a *.so
