//
// Created by Adam Naumiec on 21/03/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>



void handler(int sig, siginfo_t *info, void *context) {
    printf("Signal received: %d\n", sig);
    printf("PID of sending process: %d\n", info->si_pid);
}


void test_SA_SIGINFO() {
    printf("Testing SA_SIGINFO...\n");

    struct sigaction sa;
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) {
        raise(SIGUSR1);
        printf("Child process did not terminate\n");
        exit(0);
    }
}


void test_SA_ONSTACK() {
    printf("Testing SA_ONSTACK...\n");

    stack_t ss;
    ss.ss_sp = malloc(SIGSTKSZ);
    ss.ss_size = SIGSTKSZ;
    ss.ss_flags = 0;

    if (sigaltstack(&ss, NULL) == -1) {
        perror("sigaltstack");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_ONSTACK;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) {
        raise(SIGUSR1);
        printf("Child process did not terminate\n");
        exit(0);
    }
}


void test_SA_NODEFER() {
    printf("Testing SA_NODEFER...\n");

    struct sigaction sa;
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_NODEFER;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) {
        raise(SIGUSR1);
        printf("Child process did not terminate\n");
        exit(0);
    }
}


int main(int argc, char *argv[]) {
    struct sigaction sa;
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO;

    test_SA_SIGINFO();
    test_SA_NODEFER();
    test_SA_ONSTACK();

    return 0;
}
