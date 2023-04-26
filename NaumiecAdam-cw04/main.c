#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define MAX_MODES 5




volatile sig_atomic_t sigusr1_received = 0;


void sigusr1_handler(int sig, siginfo_t *info, void *ucontext) {
    sigusr1_received = 1;
}


void send_mode(int catcher_pid, int mode) {
    union sigval value;
    value.sival_int = mode;
    if (sigqueue(catcher_pid, SIGUSR1, value) == -1) {
        perror("Error sending signal to catcher");
        exit(EXIT_FAILURE);
    }
}


void wait_for_ack() {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigsuspend(&mask);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Wrong arguments", argv[0]);
        return 1;
    }

    int catcher_pid = atoi(argv[1]);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = sigusr1_handler;
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Error setting signal handler for SIGUSR1");
        return 1;
    }

    int num_modes = argc - 2;
    for (int i = 0; i < num_modes; i++) {
        int mode = atoi(argv[i + 2]);
        send_mode(catcher_pid, mode);
        wait_for_ack();
    }

    while (!sigusr1_received) {
        sleep(1);
    }

    printf("All modes received by catcher\n");

    return 0;
}
