#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  // Задание 1 - просто копирование кода 03-1.с?
  pid_t pid, ppid;

  fork();

  pid  = getpid();
  ppid = getppid();

  printf("My pid = %d, my ppid = %d\n", (int)pid, (int)ppid);

  return 0;
}
