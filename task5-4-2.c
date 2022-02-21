#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Программа читает из fifo.
int main()
{
  int     fd;
  char    resstring[14];
  char    name[] = "aaa.fifo";

  (void)umask(0);

  fd = open(name, O_RDONLY);

  read(fd, resstring, 14);

  printf("Process_2 exit, resstring: %s\n", resstring);

  close(fd);

  return 0;
}
