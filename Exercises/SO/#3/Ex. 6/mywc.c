#include <stdlib.h>
#include <stdio.h>
#define BUFFER_SIZE 1

int main(int argc, char* argv[]) {
    if (argc < 2) exit(EXIT_FAILURE);       // Exception

    FILE* file = fopen(argv[2], "r");   // Only read permissions
    int result = 1;
    char buffer[BUFFER_SIZE];

    if (file == NULL) exit(EXIT_FAILURE);   // Exception
    if (argv[1][0] != '-') exit(EXIT_FAILURE);  // Exception
    
    switch (argv[1][1]) {
    case 'c':
        while(fread(buffer, sizeof(char), BUFFER_SIZE, file) > 0) {
            result++;
        }
        break;
    
    case 'w':
        while(fread(buffer, sizeof(char), BUFFER_SIZE, file) > 0) {
            if (buffer[0] == ' ') result++;
        }
        break;

    case 'l':
        while(fread(buffer, sizeof(char), BUFFER_SIZE, file) > 0) {
            if (buffer[0] == '\n') result++;
        }
        break;

    default:
        exit(EXIT_FAILURE);
    }

    fclose(file);
    printf("%d\n", result);
    exit(EXIT_SUCCESS);
}