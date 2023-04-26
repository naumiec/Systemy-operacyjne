//
// Created by Adam Naumiec on 28/03/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define BUFFER_SIZE 1025

// Zmodyfikowane zadanie -> użycie fortune i cowsay



void get_fortune(char *buffer) {
    FILE *fortune_stream = popen("fortune", "r");

    fread(buffer, sizeof(char), BUFFER_SIZE, fortune_stream);

    pclose(fortune_stream);
}


void do_cowsay(char *fortune) {
    FILE *cowsay_input = popen("cowsay", "w");

    fwrite(fortune, sizeof(char), strlen(fortune), cowsay_input);

    pclose(cowsay_input);
}


int main(int argc, char **argv) {
    if (argc != 1) {
        printf("Usage: %s\n", argv[0]);
        return 1;
    }

    char *buffer = malloc(BUFFER_SIZE);

    get_fortune(buffer);
    do_cowsay(buffer);

    free(buffer);

    return 0;
}
