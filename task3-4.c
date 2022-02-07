#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
  pid_t pid;
  printf("The child process will exec \"echo\".\n");

  pid = fork();
  int status;

  if (pid == -1) {
    printf("Error while forking.\n");
  } else if (pid == 0) {
    execl("/bin/echo", "echo", "this is", "an echo", "message.", NULL);
  } else {
    printf("Parent finished working.\n");
    //exit(-1);
    waitpid(pid, &status, 0);
  }

  return 0;
}
