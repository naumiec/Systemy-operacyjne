//
// Created by Adam Naumiec on 21/03/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

volatile sig_atomic_t mode = 1;



void print_numbers(void) {
    for (int i = 1; i <= 100; i++) {
        printf("%d\n", i);
    }
}


void print_time(void) {
    time_t now = time(NULL);
    printf("Current time: %s", ctime(&now));
}


void print_num_requests(int num_requests) {
    printf("Number of mode change requests: %d\n", num_requests);
}


void print_time_periodically(int *continue_flag) {
    while (*continue_flag) {
        time_t now = time(NULL);
        printf("Current time: %s", ctime(&now));
        sleep(1);
    }
}


void sigusr1_handler(int sig, siginfo_t *siginfo, void *context) {
    printf("Catcher received SIGUSR1 signal from sender (PID: %d)\n", siginfo->si_pid);
    mode = siginfo->si_value.sival_int;
    printf("New mode: %d\n", mode);

    union sigval value;
    value.sival_int = 1;
}


int main(int argc, char *argv[]) {
    printf("Catcher PID: %d\n", getpid());

    struct sigaction sa;
    sa.sa_sigaction = sigusr1_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    struct sigaction term_sa;
    term_sa.sa_handler = SIG_DFL;
    sigemptyset(&term_sa.sa_mask);
    term_sa.sa_flags = 0;

    if (sigaction(SIGTERM, &term_sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    int num_requests = 0;
    int continue_flag = 1;

    while (continue_flag) {
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigsuspend(&mask);

        switch (mode) {
            case 1:
                print_numbers();
                break;
            case 2:
                print_time();
                break;
            case 3:
                print_num_requests(num_requests);
                num_requests++;
                break;
            case 4:
                print_time_periodically(&continue_flag);
                break;
            case 5:
                continue_flag = 0;
                break;
            default:
                printf("Invalid mode: %d\n", mode);
                break;
        }
    }

    printf("Catcher has received mode 5, exiting...\n");

    return 0;
}
