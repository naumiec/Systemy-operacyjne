CC=gcc
CFLAGS=-Wall -ggdb -std=c11
TASK=mail



all:
	make $(TASK)
	make clean

$(TASK): $(TASK).c
	$(CC) $(CFLAGS) -c $(TASK).c -o $(TASK).o

clean:
	rm -f $(TASK) *.o *.exe *.out *.a *.so
