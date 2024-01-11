// Fix: MyHistory command

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#define MAX_ARGS 1024
#define MAX_HISTORY 10

// Separate command and arguments using strtok()
void get_arguments(char* command, char* args[MAX_ARGS]) {
  char* token = strtok(command, " ");
  int i = 0;
  while (token != NULL) {
    args[i] = token;
    token = strtok(NULL, " ");
  }
}

// Exit command
void try_exit_command(char* command) {
  if (strcmp(command, "exit") == 0) exit(EXIT_SUCCESS);
}

// History command using tail
bool try_history_command(char* args[], char* history[], int history_index) {
  if (strcmp(args[0], "myhistory") != 0) return false;

  int n = (args[1] != NULL) ? atoi(args[1]) : history_index;
  n = (n > history_index) ? history_index : n;

  for (int i = 0; i < n; i++) {
    int index = (history_index - i - 1 + MAX_HISTORY) % MAX_HISTORY;
    if (history[index] != NULL) {
      fprintf(stdout, "%s\n", history[index]);
    }
  }

  return true;
}

// Add command to history
void add_to_history(char* command, char* history[], int history_index) {
  history[history_index] = command;
  history_index = (history_index + 1) % MAX_HISTORY;
}

// ============================================================

int main(int argc, char *argv[]) {
  char* history[MAX_HISTORY] = {NULL};
  int history_index = 0;

  for (;;) {
    /* give prompt, read command and null terminate it */
    fprintf(stdout, "$ ");
    char buf[1024];
    char *command = fgets(buf, sizeof(buf), stdin);
    if (command == NULL) break;
    command[strlen(buf) - 1] = '\0';

    // ========================================================

    char* args[MAX_ARGS] = {NULL};
    get_arguments(command, args);
    try_exit_command(command);
    if (try_history_command(args, history, history_index)) continue;
    add_to_history(command, history, history_index);

    // ========================================================

    /* call fork and check return value */
    pid_t pid = fork();
    if (pid == -1) {perror("fork"); exit(EXIT_FAILURE);}

    if (pid == 0) {
      /* child */
      int retv = execvp(args[0], args); // Modified
      if (retv == -1) {perror("execlp"); exit(EXIT_FAILURE);}
    }

    /* shell waits for command to finish before giving prompt again */
    int retv = waitpid(pid, NULL, 0);
    if (retv == -1) {
      perror("waitpid");
      exit(EXIT_FAILURE);
    }
  }

  exit(EXIT_SUCCESS);
}
