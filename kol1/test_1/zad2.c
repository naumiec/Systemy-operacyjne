#include "zad2.h"



void mask() {
    /*  Zamaskuj sygnał SIGUSR2, tak aby nie docierał on do procesu */

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("sigprocmask error");
    }

    check_mask();
}


void process() {
    /*  Stworz nowy process potomny i uruchom w nim program ./check_fork
        W procesie macierzystym:
            1. poczekaj 1 sekundę
            2. wyślij SIGUSR1 do procesu potomnego
            3. poczekaj na zakończenie procesu potomnego */

    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1) {
        perror("fork error");

    } else if (pid == 0) {
        char *args[] = {"./check_fork", NULL};
        execve(args[0], args, NULL);
        perror("error");

    } else {
        sleep(1);

        if (kill(pid, SIGUSR1) == -1) {
            perror("kill error");
        }

        if (wait(&status) == -1) {
            perror("wait error");
        }
    }
}


int main() {
    mask();
    process();

    return 0;
}
