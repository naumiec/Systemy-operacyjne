//
// Created by Adam Naumiec on 21/03/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>



void sigusr1_handler(int signum) {
    printf("Received SIGUSR1 signal\n");
}


void test_ignore() {
    printf("Testing ignore...\n");

    signal(SIGUSR1, SIG_IGN);

    if (fork() == 0) {
        raise(SIGUSR1);
        printf("Child process did not terminate\n");
        exit(0);
    }

    sleep(1);
    printf("Parent process terminated\n");
}


void test_handler() {
    printf("Testing handler...\n");

    signal(SIGUSR1, sigusr1_handler);

    if (fork() == 0) {
        raise(SIGUSR1);
        printf("Child process did not terminate\n");
        exit(0);
    }

    sleep(1);
    printf("Parent process terminated\n");
}


void test_mask() {
    printf("Testing mask...\n");

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    if (fork() == 0) {
        raise(SIGUSR1);
        if (sigismember(&mask, SIGUSR1))
            printf("Child process mask includes SIGUSR1\n");
        else
            printf("Child process mask does not include SIGUSR1\n");
        exit(0);
    }

    sleep(1);
    printf("Parent process terminated\n");
}


void test_pending() {
    printf("Testing pending...\n");

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    if (fork() == 0) {
        sigset_t pending;
        sigpending(&pending);
        if (sigismember(&pending, SIGUSR1))
            printf("Child process has pending SIGUSR1\n");
        else
            printf("Child process does not have pending SIGUSR1\n");
        exit(0);
    }

    raise(SIGUSR1);

    sleep(1);
    printf("Parent process terminated\n");
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [ignore|handler|mask|pending]\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "ignore") == 0) {
        test_ignore();
    } else if (strcmp(argv[1], "handler") == 0) {
        test_handler();
    } else if (strcmp(argv[1], "mask") == 0) {
        test_mask();
    } else if (strcmp(argv[1], "pending") == 0) {
        test_pending();
    } else {
        printf("Invalid argument\n");
        exit(1);
    }

    return 0;
}
