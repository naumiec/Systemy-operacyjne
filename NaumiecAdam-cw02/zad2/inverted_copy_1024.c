//
// Created by Adam Naumiec on 08/03/2023.
//
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>

#define BLOCK_SIZE 1024



int main(int argc, char** argv) {
    char buffer[BLOCK_SIZE];
    struct timespec timeStart, timeEnd;

    if (argc != 3) {
        printf("Invalid arguments\n");
        return 1;
    }

    clock_gettime(CLOCK_REALTIME, &timeStart);
    FILE* inFile = fopen(argv[1], "r");
    FILE* outFile = fopen(argv[2], "w");

    if (inFile == NULL) {
        printf("Cannot open input file\n");
        return 1;
    }

    if (outFile == NULL) {
        printf("Cannot open output file\n");
        return 1;
    }

    if (fseek(inFile, -BLOCK_SIZE, SEEK_END)) {
        printf("Cannot seek to the end of the file\n");
        return 1;
    }

    int block = fread(buffer, sizeof(char), BLOCK_SIZE, inFile);

    while (block == BLOCK_SIZE) {
        for (int i = 0; i < BLOCK_SIZE / 2; i++) {
            char tmp = buffer[i];
            buffer[i] = buffer[BLOCK_SIZE - i - 1];
            buffer[BLOCK_SIZE - i - 1] = tmp;
        }

        fwrite(buffer, sizeof(char), BLOCK_SIZE, outFile);
        block = fread(buffer, sizeof(char), BLOCK_SIZE, inFile);
    }

    if (block > 0) {
        int remaining = block;

        while (remaining > 0) {
            int chunkSize = remaining < BLOCK_SIZE ? remaining : BLOCK_SIZE;
            remaining -= chunkSize;

            for (int i = 0; i < chunkSize / 2; i++) {
                char tmp = buffer[i];
                buffer[i] = buffer[chunkSize - i - 1];
                buffer[chunkSize - i - 1] = tmp;
            }

            fwrite(buffer, sizeof(char), chunkSize, outFile);
        }
    }

    fclose(inFile);
    fclose(outFile);
    clock_gettime(CLOCK_REALTIME, &timeEnd);

    printf("Time (1024): %ld", timeEnd.tv_nsec - timeStart.tv_nsec);

    return 0;
}
