//
// Created by Adam Naumiec on 28/02/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dlfcn.h>

#include "library.h"

#define BUFFER_SIZE 1025

char command[BUFFER_SIZE] = "";



int main(int argc, char** argv) {
    libraryMemory* memory = NULL;
    memory = malloc(sizeof(libraryMemory));

    struct timespec startReal, endReal;
    struct timespec startUser, endUser;
    struct timespec startSystem, endSystem;

    while (1) {
        printf("Enter command: ");
        fgets(command, BUFFER_SIZE, stdin);

        if (strcmp(command, "exit") == 0) {
            break;

        } else if (strcmp(command, "init") == 0) {
            fgets(command, BUFFER_SIZE, stdin);
            int size = atoi(command); // wczytanie rozmiaru tablicy

            clock_gettime(CLOCK_REALTIME, &startReal);
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startUser);
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &startSystem);
            libraryInit(memory, size);

        } else if (strcmp(command, "count") == 0) {
            fgets(command, BUFFER_SIZE, stdin);
            char* file = command; // wczytanie nazwy pliku

            clock_gettime(CLOCK_REALTIME, &startReal);
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startUser);
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &startSystem);
            libraryCount(memory, file);

        } else if (strcmp(command, "show") == 0) {
            fgets(command, BUFFER_SIZE, stdin);
            int index = atoi(command); // wczytanie indeksu do wyświetlenia

            clock_gettime(CLOCK_REALTIME, &startReal);
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startUser);
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &startSystem);
            libraryGetIndex(memory, index);

        } else if (strcmp(command, "delete") == 0) {
            fgets(command, BUFFER_SIZE, stdin);
            int index = atoi(command); // wczytanie indeksu do usunięcia

            clock_gettime(CLOCK_REALTIME, &startReal);
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startUser);
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &startSystem);
            libraryDeleteIndex(memory, index);

        } else if (strcmp(command, "destroy") == 0) {

            clock_gettime(CLOCK_REALTIME, &startReal);
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startUser);
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &startSystem);
            libraryDeleteArray(memory);

        } else {
            printf("WRONG COMMAND");
        }

        clock_gettime(CLOCK_REALTIME, &endReal);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endUser);
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &endSystem);

        printf("Real time: %f\n", (double) (endReal.tv_nsec - startReal.tv_nsec) / 1000000000);
        printf("User time: %f\n", (double) (endUser.tv_nsec - startUser.tv_nsec) / 1000000000);
        printf("System time: %f\n", (double) (endSystem.tv_nsec - startSystem.tv_nsec) / 1000000000);
    }
}
