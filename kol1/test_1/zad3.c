#include "zad3.h"



void readwrite(int pd, size_t block_size);


void createpipe(size_t block_size) {
    /* Utwórz potok nienazwany */
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }

    int read_fd = fd[0];
    int write_fd = fd[1];

    /* Odkomentuj poniższe funkcje zamieniając ... na deskryptory potoku */
    check_pipe(&read_fd);
    check_write(&write_fd, block_size, readwrite);
}


void readwrite(int write_pd, size_t block_size) {
    /* Otworz plik `unix.txt`, czytaj go po `block_size` bajtów
    i w takich `block_size` bajtowych kawałkach pisz do potoku `write_pd`.*/
    char* filename = "unix.txt";
    //int fd = open(filename, O_RDONLY);
    FILE* fd = fopen(filename, "r");
    char buf[block_size];
    ssize_t nread;

    if (fd == -1) {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    while ((nread = read(fd, buf, block_size)) > 0)
        if (write(write_pd, buf, nread) != nread) {
            perror("write error");
            exit(EXIT_FAILURE);
        }

    if (nread == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }

    /* Zamknij plik */
    fclose(fd);
}


int main() {
    srand(42);
    size_t block_size = rand() % 128;
    createpipe(block_size);

    return 0;
}
