/* Is it possible to catch a SIGKILL signal? 
 * This signal cannot be caught or ignored according to POSIX standards. 
 * The operating system uses SIGKILL to terminate processes that cannot be stopped in other ways. 
 * Therefore, the attempt to catch SIGKILL will always fail.
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void handler1() { printf("caught SIGTSTP\n"); }
static void handler2() { printf("caught SIGINT\n"); }
static void handler3() { printf("caught SIGHUP\n"); }

int main(int argc, char *argv[])
{
  printf("my PID is %d\n", getpid());
  if (signal(SIGTSTP, handler1) == SIG_ERR)
  {
    fprintf(stderr, "Can’t catch SIGTSTP: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }
  if (signal(SIGINT, handler2) == SIG_ERR)
  {
    fprintf(stderr, "Can’t catch SIGINT: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }
  if (signal(SIGHUP, handler3) == SIG_ERR)
  {
    fprintf(stderr, "Can’t catch SIGHUP: %s", strerror(errno));
    exit(EXIT_FAILURE);
  }

  /* stick around ... */
  for (;;)
    pause();
}
