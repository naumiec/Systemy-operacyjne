//
// Created by Adam Naumiec on 07/03/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1025



int main(int argc, char** argv) {
    char buffer[BUFFER_SIZE];
    struct timespec timeStart, timeEnd;

    if (argc != 5) {
        printf("Invalid args\n");
        return 1;
    }

    char char1 = argv[1][0];
    char char2 = argv[2][0];
    char* inputFilename = argv[3];
    char* outputFilename = argv[4];

    // ---------- ---------- ---------- ---------- ----------

    clock_gettime(CLOCK_REALTIME, &timeStart);
    FILE* LIBinputFile = fopen(inputFilename, "r");
    FILE* LIBoutputFile = fopen(outputFilename, "w");

    if (LIBinputFile == NULL) {
        printf("Cannot open input file: %s\n", inputFilename);
        return 1;
    }

    if (LIBoutputFile == NULL) {
        printf("Cannot open output file: %s\n", outputFilename);
        return 1;
    }

    size_t fileRead;
    while ((fileRead = fread(buffer, 1, BUFFER_SIZE, LIBinputFile)) > 0) {
        for (int i = 0; i < fileRead; i++) {
            if (buffer[i] == char1) {
                buffer[i] = char2;
            }
        }

        fwrite(buffer, 1, fileRead, LIBoutputFile);
    }

    fclose(LIBinputFile);
    fclose(LIBoutputFile);
    clock_gettime(CLOCK_REALTIME, &timeEnd);

    printf("Time (lib): %ld\n", timeEnd.tv_nsec - timeStart.tv_nsec);

    // ---------- ---------- ---------- ---------- ----------

    clock_gettime(CLOCK_REALTIME, &timeStart);
    int SYSinputFile = open(inputFilename, O_RDONLY);
    int SYSoutputFile = open(outputFilename, O_CREAT | O_WRONLY | O_TRUNC);

    if (SYSinputFile == -1) {
        printf("Cannot open input file: %s\n", inputFilename);
        return 1;
    }

    if (SYSoutputFile == -1) {
        printf("Cannot open output file: %s\n", outputFilename);
        return 1;
    }

    size_t SYSfileRead;
    while ((SYSfileRead = read(SYSinputFile, buffer, BUFFER_SIZE)) > 0) {
        for (size_t i = 0; i < SYSfileRead; i++) {
            if (buffer[i] == char1) {
                buffer[i] = char2;
            }
        }
        if (write(SYSoutputFile, buffer, SYSfileRead) != SYSfileRead) {
            printf("Cannot write output file: %s\n", outputFilename);
            return 1;
        }
    }
    if (SYSfileRead == -1) {
        printf("Cannot read input file: %s\n", inputFilename);
        return 1;
    }

    close(SYSinputFile);
    close(SYSoutputFile);
    clock_gettime(CLOCK_REALTIME, &timeEnd);

    printf("Time (sys): %ld\n", timeEnd.tv_nsec - timeStart.tv_nsec);

    return 0;
}
