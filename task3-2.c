#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
  pid_t pid, ppid;
  printf("The parent process will have line \"is_child = 0\",\nthe child will have line \"is_child = 1\".\n");

  pid = fork();
  int is_child = -1;
  int status;

  if (pid == -1) {
    printf("Error while forking.\n");
  } else if (pid == 0) {
    is_child = 1;
  } else {
    // Нашел функцию waitpid.
    // В этом случае ppid ребенка будет совпадать с pid родителя. Вроде проверил.
    waitpid(pid, &status, 0);
    is_child = 0;
  }

  pid  = getpid();
  ppid = getppid();

  printf("My pid = %d, my ppid = %d, is_child = %d\n", (int)pid, (int)ppid, is_child);

  return 0;
}
