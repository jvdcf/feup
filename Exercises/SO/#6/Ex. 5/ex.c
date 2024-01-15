#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int flag = 1;
static void handler1() { exit(EXIT_SUCCESS); }

int main(int argc, char *argv[]) {
    printf("my PID is %d\n", getpid());

    if (signal(SIGINT, handler1) == SIG_ERR) {
        fprintf(stderr, "Can’t catch SIGINT: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* enter potentially infinite loop ... */
    while (flag)
        pause();
    exit(EXIT_SUCCESS);
}