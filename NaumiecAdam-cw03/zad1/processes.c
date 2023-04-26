//
// Created by Adam Naumiec on 14/03/2023.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>



int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments!\n");
        return 1;
    }

    int numProcesses = atoi(argv[1]);

    if (numProcesses < 1) {
        fprintf(stderr, "Wrong number of processes!\n");
        return 1;
    }

    for (int i = 0; i < numProcesses; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            printf("Parent process id: %d, child process id: %d\n", getppid(), getpid());
            return 0;
        } else if (pid < 0) {
            fprintf(stderr, "Fork failed!\n");
            return 1;
        }
    }

    while (wait(NULL) > 0);
    printf("argv[1]: %d\n", numProcesses);

    return 0;
}