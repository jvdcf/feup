// DONE: Father process reads a file and sends the contents to the child process.
//       The child process receives the contents, converts them to uppercase and sends them back to the father process.
//       The father process receives the contents and prints them on the screen.

#include <sys/wait.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h> // toupper
#define MAX 1024

int main(int argc, char *argv[]) {
  if (argc != 2) exit(EXIT_FAILURE);

  char buf[MAX];
  int sockets[2], retv;
  retv = socketpair(AF_UNIX, SOCK_STREAM, 0, sockets);
  if (retv == -1) {perror("socketpair"); exit(EXIT_FAILURE);}

  retv = fork();
  if (retv == -1) {perror("fork"); exit(EXIT_FAILURE);}

  if (retv > 0) { /* parent */
    // Read a file and send the contents to the child process
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {perror("fopen"); exit(EXIT_FAILURE);}
    char string1[MAX];
    fread(string1, sizeof(char), MAX, file);
    fclose(file);

    close(sockets[1]);
    write(sockets[0], string1, sizeof(string1));
    read(sockets[0], buf, sizeof(buf));
    printf("Modified message: -->\n%s\n", buf);
    close(sockets[0]);
    retv = wait(NULL);
    if (retv == -1) {perror("wait"); exit(EXIT_FAILURE);}

  } else { /* child */    
    close(sockets[0]);
    read(sockets[1], buf, sizeof(buf));
    printf("Original message: -->\n%s\n", buf);

    // Convert to uppercase
    for (int i = 0; buf[i] != '\0'; i++) {
      buf[i] = toupper(buf[i]);
    }

    write(sockets[1], buf, sizeof(buf));
    close(sockets[1]);
    exit(EXIT_SUCCESS);
  }
}
