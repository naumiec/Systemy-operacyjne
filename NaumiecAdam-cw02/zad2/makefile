CC=gcc
CFLAGS=-Wall -g0 -std=c11
TASK=inverted_copy
TASK_1024=inverted_copy_1024



all:
	make $(TASK)
	make clean

$(TASK): $(TASK).c
	$(CC) $(CFLAGS) -c $(TASK).c
	$(CC) $(CFLAGS) -o $(TASK) $(TASK).o

$(TASK_1024): $(TASK_1024).c
	$(CC) $(CFLAGS) -c $(TASK_1024).c
	$(CC) $(CFLAGS) -o $(TASK_1024) $(TASK_1024).o

save:
	echo "Zadanie 2 - pomiar" > ./pomiar_zad_2.txt
	$(CC) $(CFLAGS) $(TASK).c -o $(TASK)
	./$(TASK) in.txt out.txt >> ./pomiar_zad_2.txt
	$(CC) $(CFLAGS) $(TASK_1024).c -o $(TASK_1024)
	./$(TASK_1024) in.txt out.txt >> ./pomiar_zad_2.txt
	make clean

clean:
	rm -f $(TASK) *.o *.exe *.out *.a *.so *.in *.exe
