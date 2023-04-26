//
// Created by Adam Naumiec on 28/03/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>



double f(double x) {
    return 4.0 / (1.0 + x * x);
}


double calculateIntegral(double start, double end, double width) {
    double sum = 0;
    double x;

    for (x = start; x < end; x += width) {
        sum += f(x);
    }

    return sum * width;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <width> <numProcs>\n", argv[0]);
        return 1;
    }

    double width = atof(argv[1]);
    int numProcs = atoi(argv[2]);

    if (numProcs == -1 && width == -1) {
        // default values
        numProcs = 5;
        width = 0.000001;
    } else {
        if (numProcs <= 0) {
            printf("Number of processes must be greater than 0.\n");
            return 1;
        }

        if (width <= 0) {
            printf("Width must be greater than 0.\n");
            return 1;
        }
    }

    struct timespec timeStart, timeEnd;

    int i, pid;
    double start, end, result, totalResult = 0;
    int pipeFd[numProcs][2];

    clock_gettime(CLOCK_MONOTONIC, &timeStart);

    for (i = 0; i < numProcs; i++) {
        start = i * (1.0 / numProcs);
        end = (i + 1) * (1.0 / numProcs);

        if (pipe(pipeFd[i]) == -1) {
            perror("pipe");
            exit(1);
        }

        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            close(pipeFd[i][0]);

            result = calculateIntegral(start, end, width);

            if (write(pipeFd[i][1], &result, sizeof(double)) == -1) {
                perror("write");
                exit(1);
            }

            close(pipeFd[i][1]);

            exit(0);
        } else {
            close(pipeFd[i][1]);

            if (read(pipeFd[i][0], &result, sizeof(double)) == -1) {
                perror("read");
                exit(1);
            }

            close(pipeFd[i][0]);

            totalResult += result;

            wait(NULL);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &timeEnd);

    printf("The result is: %f\n", totalResult);
    printf("Time taken: %f seconds\n",
           (timeEnd.tv_sec - timeStart.tv_sec) + (timeEnd.tv_nsec - timeStart.tv_nsec) / 1000000000.0);

    return 0;
}
