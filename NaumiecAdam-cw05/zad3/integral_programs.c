#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_PATH "/tmp/integral_queue"
#define BUFFER_SIZE 1025

char writeBuffer[BUFFER_SIZE] = "";
char readBuffer[BUFFER_SIZE] = "";



int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: ./main <dx> <n>\n");
        return 1;
    }

    if (access("./worker", F_OK) == -1) {
        printf("Worker not found\n");
        return 1;
    }

    double dx = strtod(argv[1], NULL);
    int n = atoi(argv[2]);

    if (dx == -1 && n == -1) {
        // default values

        dx = 0.0000001;
        n = 10;
    }

    struct timespec timeStart, timeEnd;

    clock_gettime(CLOCK_REALTIME, &timeStart);

    double totalResult = 0.0;
    double width = 1.0 / n;

    mkfifo(FIFO_PATH, 0666);

    for (int i = 0; i < n; i++) {
        char arg2[BUFFER_SIZE];
        char arg3[BUFFER_SIZE];

        char *args[] = {"worker", argv[1], arg2, arg3, NULL};

        snprintf(arg2, BUFFER_SIZE, "%lf", i * width);
        snprintf(arg3, BUFFER_SIZE, "%lf", (i + 1) * width);

        if (!fork()) {
            execv("./worker", args);
        }
    }

    int fifo = open(FIFO_PATH, O_RDONLY);

    int j = 0;
    while (j < n) {
        size_t size = read(fifo, readBuffer, BUFFER_SIZE);
        readBuffer[size] = 0;

        char delim[] = "\n";
        char *token;

        token = strtok(readBuffer, delim);
        for (; token; token = strtok(NULL, delim)) {
            totalResult += strtod(token, NULL);
            j++;
        }
    }

    close(fifo);

    remove(FIFO_PATH);

    clock_gettime(CLOCK_REALTIME, &timeEnd);

    printf("The totalResult is: %f\n", totalResult);
    printf("Time taken: %f seconds\n",
           (timeEnd.tv_sec - timeStart.tv_sec) + (timeEnd.tv_nsec - timeStart.tv_nsec) / 1000000000.0);

    return 0;
}
