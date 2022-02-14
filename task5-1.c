#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
  int     fd;
  size_t  size;
  char    string[61];

  (void)umask(0);

  fd = open("readfile_task5-1.txt", O_RDONLY);

  read(fd, string, 61);
  printf("String from file: %s", string);

  close(fd);

  return 0;
}
