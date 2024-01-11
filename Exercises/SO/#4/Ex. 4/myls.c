/* How can you know if the name given as an argument is a file or a directory? 
 * Using the flag st_mode of the structure stat:
 *  if (S_ISDIR(buf.st_mode)) printf ("d");
 *  else printf ("-");
 */

// Implement a command similar to ls -l.
// total <total size>
// <permissions> <number of links> <owner> <group> <size> <modification date> <name>

#include <stdio.h>  // printf
#include <stdlib.h> // exit
#include <dirent.h> 
#include <sys/stat.h>   // stat
#include <time.h>   // ctime

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf (stderr, "usage: %s dirname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    DIR *q = opendir(argv[1]);
    if (q == NULL) {
        fprintf (stderr, "cannot open directory: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Print total size used
    struct dirent *p = readdir(q);
    int size = 0;
    while (p != NULL) {
        struct stat buf;
        if (stat(p->d_name, &buf)) {
            fprintf (stderr, "cannot stat file: %s\n", p->d_name);
            exit(EXIT_FAILURE);
        }
        size += buf.st_size;
        p = readdir(q);
    }
    printf ("total %d\n", size / 1024);

    // Print all files and directories
    rewinddir(q);
    p = readdir(q);

    while (p != NULL) {
        struct stat buf;
        if (stat(p->d_name, &buf)) {
            fprintf (stderr, "cannot stat file: %s\n", p->d_name);
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(buf.st_mode)) printf ("d");
        else printf ("-");

        if (buf.st_mode & S_IRUSR) printf ("r");
        else printf ("-");

        if (buf.st_mode & S_IWUSR) printf ("w");
        else printf ("-");

        if (buf.st_mode & S_IXUSR) printf ("x");
        else printf ("-");

        if (buf.st_mode & S_IRGRP) printf ("r");
        else printf ("-");

        if (buf.st_mode & S_IWGRP) printf ("w");
        else printf ("-");

        if (buf.st_mode & S_IXGRP) printf ("x");
        else printf ("-");

        if (buf.st_mode & S_IROTH) printf ("r");
        else printf ("-");

        if (buf.st_mode & S_IWOTH) printf ("w");
        else printf ("-");

        if (buf.st_mode & S_IXOTH) printf ("x");
        else printf ("-");

        printf(".");

        printf (" %ld", buf.st_nlink);
        printf (" %d", buf.st_uid);
        printf (" %d", buf.st_gid);
        printf (" %ld", buf.st_size);
        printf (" %s %s\n", ctime(&buf.st_ctime), p->d_name);

        p = readdir(q);
    }

    closedir(q);
    exit(EXIT_SUCCESS);
}
