CC=gcc
CFLAGS=-Wall -g0 -std=c11
TASK=change_character



all:
	make $(TASK)
	make clean

$(TASK): $(TASK).c
	$(CC) $(CFLAGS) -c $(TASK).c
	$(CC) $(CFLAGS) -o $(TASK) $(TASK).o

save:
	echo "Zadanie 1 - pomiar" > ./pomiar_zad_1.txt
	$(CC) $(CFLAGS) $(TASK).c -o $(TASK)
	./$(TASK) a A in.txt out.txt>> ./pomiar_zad_1.txt
	make clean

clean:
	rm -f $(TASK) *.o *.exe *.out *.a *.so *.exe
