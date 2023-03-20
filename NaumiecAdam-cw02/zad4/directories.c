//
// Created by Adam Naumiec on 10/03/2023.
//
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>

#define BUFFER_SIZE 1025



long long sum;

int directorySearch(const char* path, const struct stat* statbuf, int typeflag) {
    if (!S_ISDIR(statbuf->st_mode)) {
        printf("File: %s, size: %ld\n", path, (long)statbuf->st_size);
        sum += statbuf->st_size;
    }

    return 0;
}


int main(int argc, char** argv) {
    sum = 0;

    if (argc != 2) {
        printf("Wrong arguments\n");
        return 1;
    }

    if (ftw(argv[1], directorySearch, 1) == -1) {
        printf("Error while searching directory\n");
        return 1;
    }

    printf("Total size: %lld bytes\n", sum);

    return 0;
}
