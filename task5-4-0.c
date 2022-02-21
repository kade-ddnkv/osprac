#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

// Программа создает fifo.
int main()
{
  char    name[] = "aaa.fifo";

  (void)umask(0);

  if (mknod(name, S_IFIFO | 0666, 0) < 0) {
    printf("Can\'t create FIFO\n");
    exit(-1);
  }

  return 0;
}
