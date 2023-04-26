CC=gcc
CFLAGS=-Wall -ggdb -std=c11
TASK=integral_processes



all:
	make $(TASK)
	make clean

$(TASK): $(TASK).c
	$(CC) $(CFLAGS) -c $(TASK).c -o $(TASK).o

report:
	$(CC) $(CFLAGS) $(TASK).c -o $(TASK)
	echo "Report for $(TASK):" > report2.txt
	echo "" >> report2.txt

	echo "10 process, 0.000000001:" >> report2.txt
	./$(TASK) 0.000000001 10 >> report2.txt
	echo "" >> report2.txt

	echo "100 process, 0.000000001:" >> report2.txt
	./$(TASK) 0.000000001 100 >> report2.txt
	echo "" >> report2.txt

	echo "10 process, 0.000001:" >> report2.txt
	./$(TASK) 0.000001 10 >> report2.txt
	echo "" >> report2.txt

	echo "100 process, 0.000001:" >> report2.txt
	./$(TASK) 0.000001 100 >> report2.txt
	echo "" >> report2.txt

	make clean

clean:
	rm -f $(TASK) *.o *.exe *.out *.a *.so
