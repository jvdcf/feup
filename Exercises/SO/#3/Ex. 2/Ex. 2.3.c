#include <stdlib.h> // exit()
#include <string.h> // strlen(), strstr(), ...
#include <stdio.h>  // printf()


int main(int argc, char* argv[]) {
    if (argc == 3) {
        char* str1 = argv[1];
        char* str2 = argv[2];
        int size1 = strlen(argv[1]);
        //int size2 = strlen(argv[2]);
        int count = 0;

        while (strstr(str2, str1) != NULL) {
            str2 = strstr(str2, str1) + (size1 * sizeof(char));
            count++;
        }

        printf("%d\n", count);
        exit(EXIT_SUCCESS);

    } else {
        exit(EXIT_FAILURE);
    }

}