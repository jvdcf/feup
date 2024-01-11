#include <stdlib.h>
#include <stdio.h> // fopen(), fread(), ...
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc < 2) exit(EXIT_FAILURE);       // Exception

    for (int i = 1; i < argc; i++) {
        FILE* file = fopen(argv[1], "r");   // Only read permissions
        if (file == NULL) exit(EXIT_FAILURE);   // Exception
        char buffer[BUFFER_SIZE];
        
        int nchars = fread(buffer, sizeof(char), BUFFER_SIZE, file);    // Fill buffer
        while (nchars > 0) {
            printf("%s", buffer);
            nchars = fread(buffer, sizeof(char), BUFFER_SIZE, file);
        }
        
        printf("\n");
        fclose(file);
    }

    exit(EXIT_SUCCESS);
}