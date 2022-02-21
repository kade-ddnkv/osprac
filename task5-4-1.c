#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Программа записывает в fifo.
int main()
{
  int     fd;
  char    name[] = "aaa.fifo";

  (void)umask(0);

  fd = open(name, O_WRONLY);

  write(fd, "Hello, world!", 14);

  close(fd);

  printf("Process_1 exit.\n");

  return 0;
}
