CC=gcc
CFLAGS=-Wall -ggdb -std=c11
TASK=main



all:
	make $(TASK)
	make clean

$(TASK): $(TASK).c
	$(CC) $(CFLAGS) -c $(TASK).c
	$(CC) $(CFLAGS) -o $(TASK) $(TASK).o

clean:
	rm -f $(TASK) *.o *.exe *.out *.a *.so
