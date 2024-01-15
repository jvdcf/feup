#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    struct stat info;
    blkcnt_t count = 0;

    if (argc < 2) {
        fprintf(stderr, "usage: %s files\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Read many files
    for (int i = 1; i < argc; i++) {
        int retv = stat(argv[i], &info);
        if (retv == -1) {
            fprintf(stderr, "fsize: Can’t stat %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }

        count += info.st_blocks;
        // Last modification date and owner
        printf("%s: %s | Owned by user %d \n", argv[i], ctime(&info.st_mtime), (int)info.st_uid);
    }

    // Calculate the sum of block
    printf("Total number of blocks: %d \n", (int)count);
    exit(EXIT_SUCCESS);
}
