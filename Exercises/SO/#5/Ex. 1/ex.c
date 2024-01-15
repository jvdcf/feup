#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    fork();
    fork();
    fork();
    printf("%d\n", (int)getpid());
    exit(EXIT_SUCCESS);
}

// ----------------------------------------------
// Nº de processos criados: 8