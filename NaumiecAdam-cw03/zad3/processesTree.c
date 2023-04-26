//
// Created by Adam Naumiec on 14/03/2023.
//

// Testowaano m.in. dla katalogu ./test i sekwencji "abc"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <ftw.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>

#define BUFFER_SIZE strlen(prefix) + 1



int searchDir(char* path, char* prefix) {
    char buffer[BUFFER_SIZE];
    char resolvedPath[PATH_MAX];
    struct stat stats;
    struct dirent* entry;
    DIR* dir;
    FILE* file;

    if ((dir = opendir(path)) == NULL) {
        perror("(dir) Cannot open directory!");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        snprintf(resolvedPath, sizeof(resolvedPath), "%s/%s", path, entry->d_name);

        if (stat(resolvedPath, &stats)) {
            perror("(stat) Cannot get stats!");
            return 1;
        }

        if (S_ISDIR(stats.st_mode)) {
            // skip . and ..
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            int id = fork();

            if (id == 0) {
                searchDir(resolvedPath, prefix);
                return 0;
            }
        } else {
            file = fopen(resolvedPath, "r");

            if (file == NULL) {
                perror("(fopen) Cannot open file!");
                return 1;
            }

            size_t size = fread(buffer, sizeof(char), BUFFER_SIZE - 1, file);
            buffer[size] = 0;

            if (strcmp(buffer, prefix) == 0) {
                printf("Path: %s, pid: %d\n", resolvedPath, getpid());
                fflush(NULL);
            }
        }
    }

    return 0;
}


int main(int argc, char** argv) {
    if (argc != 3) {
        perror("Wrong number of arguments!");
        return 1;
    }

    if (strlen(argv[2]) > 255){
        perror("Prefix is too long! (max 255 characters)");
        return 1;
    }

    // 1. path, 2. prefix
    searchDir(argv[1], argv[2]);

    while(wait(NULL) > 0);

    return 0;
}