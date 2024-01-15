#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_SIZE 64

int main(int argc, char* argv[]) {
    if (argc == 2) {
        char* string = (char*)malloc(MAX_SIZE * sizeof(char));
        strcpy(string, argv[1]);

        for (int i = 0; i < MAX_SIZE; i++)
            string[i] = tolower(string[i]);
        
        printf("%s\n", string);

        free(string);
        exit(EXIT_SUCCESS);

    } else {
        exit(EXIT_FAILURE);
    }
}
