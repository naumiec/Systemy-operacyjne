CC=gcc
CFLAGS=-Wall -ggdb -std=c11
CLIENT=client_posix
SERVER=server_posix



all:
	$(CC) $(CFLAGS) -c $(CLIENT).c -o $(CLIENT).o
	$(CC) $(CFLAGS) -c $(SERVER).c -o $(SERVER).o
	make clean


clean:
	rm -f $(CLIENT) $(SERVER) *.o *.exe *.out *.a *.so
