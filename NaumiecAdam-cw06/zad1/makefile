CC=gcc
CFLAGS=-Wall -ggdb -std=c11
CLIENT=client_systemv
SERVER=server_systemv



all:
	$(CC) $(CFLAGS) -c $(CLIENT).c -o $(CLIENT).o
	$(CC) $(CFLAGS) -c $(SERVER).c -o $(SERVER).o
	make clean


clean:
	rm -f $(CLIENT) $(SERVER) *.o *.exe *.out *.a *.so
