//
// Created by Adam Naumiec on 14/03/2023.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>



int main(int argc, char** argv) {
    setbuf(stdout, NULL);

    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments!\n");
        return 1;
    }

    char* path = (argv[1]);

    printf("Name of the program: %s ", argv[0]);
    execl("/bin/ls", "ls", "-l", path, NULL);

    return 0;
}