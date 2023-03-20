//
// Created by Adam Naumiec on 09/03/2023.
//
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>

#define BUFFER_SIZE 1025



int main(int argc, char** argv) {
    long long sum = 0;
    struct stat stats;
    struct dirent* entry;

    char* path = ".";
    DIR* dir = opendir(path);


    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            snprintf(path, sizeof(path), "%s/%s", path, entry->d_name);

            if (stat(path, &stats) == 0) {
                if (!S_ISDIR(stats.st_mode)) {
                    printf("Name: %s, size: %lld\n", entry->d_name, stats.st_size);
                }
            } else {
                fprintf(stderr, "Cannot get stats for %s\n", path);
            }
        }

        printf("Total size: %lld bytes\n", sum);
        closedir(dir);
    } else {
        printf("Cannot open directory");
    }

    return 0;
}
