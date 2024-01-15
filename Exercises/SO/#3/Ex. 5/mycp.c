#include <stdlib.h>
#include <stdio.h>
#define BUFFER_SIZE 1

int main(int argc, char* argv[]) {
    if (argc < 3) exit(EXIT_FAILURE);       // Exception

    FILE* file1 = fopen(argv[1], "r");   // Only read permissions
    if (file1 == NULL) exit(EXIT_FAILURE);   // Exception
    FILE* file2 = fopen(argv[2], "w");   // Only write permissions (also clears file)
    if (file2 == NULL) exit(EXIT_FAILURE);   // Exception

    char buffer[BUFFER_SIZE];
    while (fread(buffer, sizeof(char), BUFFER_SIZE, file1) > 0) {
        fwrite(buffer, sizeof(char), BUFFER_SIZE, file2);
    }

    fclose(file1);
    fclose(file2);

    exit(EXIT_SUCCESS);
}