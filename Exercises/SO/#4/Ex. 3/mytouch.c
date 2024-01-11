#include <stdlib.h>   // exit()
#include <sys/stat.h> // stat(), umask()
#include <stdio.h>    // printf()
#include <fcntl.h>    // open()
#include <unistd.h>   // close()
#include <utime.h>    // utimbuf
#include <time.h>     // time()

int main(int argc, char *argv[]) {
  if (argc != 2) exit(EXIT_FAILURE);
  struct stat st;

  // If file not exists, create it with permissions 644
  if (stat(argv[1], &st) != 0) {
    int fd = open(argv[1], O_CREAT, 0644); // or S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
    
    if (fd == -1) {
      perror("open");
      exit(EXIT_FAILURE);
    }
    
    close(fd);
    exit(EXIT_SUCCESS);
  }

  // If file exists, change the last modification time to the current local time
  struct utimbuf new_times;
    new_times.modtime = time(NULL);

    if (utime(argv[1], &new_times) != 0) {
      perror("utime");
      exit(EXIT_FAILURE);
    }
}
