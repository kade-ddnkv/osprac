#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

void my_handler(int nsig) {
  int status;
  pid_t pid;

  // waitpid, пока у процесса есть завершенные дети
  // WNOHANG - return immediately if no child has exited.
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    if ((status & 0xff) == 0) {
        printf("Process %d was exited with status %d\n", pid, status >> 8);
    } else if ((status & 0xff00) == 0) {
        printf("Process %d killed by signal %d %s\n", pid, status &0x7f,
               (status & 0x80) ? "with core file" : "without core file");
    }
  }
  if (pid < 0) {
    // После завершения все процессов почему-то срабатывает вызов printf со статусом 10.
    // (иногда 2 раза).
    // Это значит, что сигнал SIGCHLD был послан, но детей у процесса нет (статус 10).
    // Почему так? Ведь этот сигнал в маске ставится на 0 после завершения этой функции.
    printf("Some error on waitpid errno = %d\n", errno);
  }
}

int main(void) {
  pid_t pid;
  int i,j;

  (void) signal(SIGCHLD, my_handler);

  for (i=0; i<10; i++) {
    if ((pid = fork()) < 0) {
      printf("Can\'t fork child 1\n");
      exit(1);
    } else if (pid == 0) {
      /* Child  */
      for (j=1; j < 10000000; j++);
      exit(200 + i);
    }
  }

  while(1);
  return 0;
}
