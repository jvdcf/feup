#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 3) exit(EXIT_FAILURE);                          // Exception

    FILE* file = fopen(argv[2], "r");                           // Open file
    if (file == NULL) exit(EXIT_FAILURE);

    char buffer[BUFFER_SIZE];                                   // Initialize variables
    char* pointer = &buffer[0];
    char* next_found = strstr(buffer, argv[1]);
    fread(buffer, sizeof(char), BUFFER_SIZE, file);
    int column = 1;
    int line = 1;
    printf("DEGUB: Valid arguments, searching...\n");
    printf("DEBUG: File: \n%s\n", buffer);
    printf("DEBUG: Word: %s\n", argv[1]);
    printf("Next found: %s\n", next_found); // BUG: NULL

    while (next_found != NULL) {                                // For every found word
        printf("DEBUG: Found word, finding coordinates...\n");
        while (pointer != next_found) {                         // For every character before the word
            switch (*pointer) {
                case '\n':                                      // If newline
                    line++;
                    column = 1;
                    break;
                
                default:                                        // If any other character                           
                    column++;
                    break;
            }   
        }

        printf("%d:%d\n", line, column);
        next_found = strstr(next_found + 1, argv[1]);
    }

    fclose(file);                                               // Exit program safely
    exit(EXIT_SUCCESS);
}