#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_PATH "/tmp/integral_queue"
#define BUFFER_SIZE 1025
char buffer[BUFFER_SIZE] = "";



double f(double x) {
    return 4 / (x * x + 1);
}


double calculateIntegral(double a, double b, double dx) {
    double result = 0.0;
    for (double x = a; x < b; x += dx) {
        result += f(x) * dx;
    }

    return result;
}


int main(int argc, char **argv) {
    double dx = strtod(argv[1], NULL);
    double a = strtod(argv[2], NULL);
    double b = strtod(argv[3], NULL);

    double result = calculateIntegral(a, b, dx);

    // ----------

    int size = snprintf(buffer, BUFFER_SIZE, "%lf\n", result);

    if (size >= BUFFER_SIZE) {
        fprintf(stderr, "Buffer overflow\n");
        return 1;
    }

    if (size < 0) {
        perror("snprintf");
        return 1;
    }

    int fifo = open(FIFO_PATH, O_WRONLY);

    if (fifo < 0) {
        perror("open");
        return 1;
    }

    if (write(fifo, buffer, size) < 0) {
        close(fifo);
        perror("write");
        return 1;
    }

    close(fifo);

    return 0;
}
