#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc == 3) {
        char* inside = strstr(argv[2], argv[1]);
        printf("%d\n", inside != NULL);
        exit(EXIT_SUCCESS);

    } else {
        exit(EXIT_FAILURE);
    }
}