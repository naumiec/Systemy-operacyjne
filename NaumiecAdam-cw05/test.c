//
// Created by Adam Naumiec on 04/04/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

// 3. Obustronna komunikacja między dwoma procesami za pomocą 2 pipeów

int main(int argc, char* argv) {
    int pipe1[2];
    int pipe2[2];
    int pid;

    if (pipe(pipe1) < 0) {
        perror("pipe1");
        return 1;
    }

    if (pipe(pipe2) < 0) {
        perror("pipe2");
        return 1;
    }

    if ((pid = fork()) < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // child
        close(pipe1[1]);
        close(pipe2[0]);

        char buffer[1024];
        int size;

        while ((size = read(pipe1[0], buffer, 1024)) > 0) {
            buffer[size] = '\0';
            printf("Child: %s", buffer);
            write(pipe2[1], buffer, size);
        }

        close(pipe1[0]);
        close(pipe2[1]);
    } else {
        // parent
        close(pipe1[0]);
        close(pipe2[1]);

        char buffer[1024];
        int size;

        while (1) {
            printf("Parent: ");
            fgets(buffer, 1024, stdin);
            size = strlen(buffer);
            write(pipe1[1], buffer, size);
            read(pipe2[0], buffer, 1024);
            printf("Parent: %s", buffer);
        }

        close(pipe1[1]);
        close(pipe2[0]);
    }


    return 0;
}
