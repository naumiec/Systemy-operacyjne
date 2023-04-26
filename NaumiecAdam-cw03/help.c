#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <limits.h>

void traverse_dir(const char *dir_path, const char *search_str);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <dir_path> <search_str>\n", argv[0]);
        return 1;
    }
    traverse_dir(argv[1], argv[2]);
    return 0;
}

void traverse_dir(const char *dir_path, const char *search_str) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // ignore . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // build full path of the entry
        char entry_path[PATH_MAX];
        snprintf(entry_path, sizeof(entry_path), "%s/%s", dir_path, entry->d_name);

        struct stat statbuf;
        if (lstat(entry_path, &statbuf) == -1) {
            perror("lstat");
            continue;
        }

        // recursively traverse subdirectories
        if (S_ISDIR(statbuf.st_mode)) {
            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                continue;
            } else if (pid == 0) {
                traverse_dir(entry_path, search_str);
                exit(EXIT_SUCCESS);
            }
        }
            // check if the entry is a regular file and contains the search string
        else if (S_ISREG(statbuf.st_mode) && strstr(entry->d_name, search_str) != NULL) {
            FILE *fp = fopen(entry_path, "r");
            if (fp == NULL) {
                perror("fopen");
                continue;
            }
            char buf[PATH_MAX];
            if (fgets(buf, sizeof(buf), fp) != NULL && strstr(buf, search_str) != NULL) {
                printf("%s:%d\n", entry_path, getpid());
            }
            fclose(fp);
        }
    }

    if (closedir(dir) == -1) {
        perror("closedir");
    }
}
