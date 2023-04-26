CC=gcc
CFLAGS=-Wall -ggdb -std=c11
TASK=integral_programs
PROG=worker



all:
	make $(TASK)
	make $(PROG)
	make clean

$(TASK): $(TASK).c
	$(CC) $(CFLAGS) -c $(TASK).c -o $(TASK).o

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -c $(PROG).c -o $(PROG).o

report:
	$(CC) $(CFLAGS) $(TASK).c -o $(TASK)
	$(CC) $(CFLAGS) $(PROG).c -o $(PROG)

	echo "Report for $(TASK):" > report3.txt
	echo "" >> report3.txt

	echo "10 process, 0.000000001:" >> report3.txt
	./$(TASK) 0.000000001 10 >> report3.txt
	echo "" >> report3.txt

	echo "100 process, 0.000000001:" >> report3.txt
	./$(TASK) 0.000000001 100 >> report3.txt
	echo "" >> report3.txt

	echo "10 process, 0.000001:" >> report3.txt
	./$(TASK) 0.000001 10 >> report3.txt
	echo "" >> report3.txt

	echo "100 process, 0.000001:" >> report3.txt
	./$(TASK) 0.000001 100 >> report3.txt
	echo "" >> report3.txt

	make clean

clean:
	rm -f $(TASK) $(PROG) *.o *.exe *.out *.a *.so
