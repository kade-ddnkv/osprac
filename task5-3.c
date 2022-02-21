#define _GNU_SOURCE
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int fd[2];
  int curr_size;
  int sum_size = 0;

  (void)umask(0);

  pipe2(fd, O_NONBLOCK);

  while(1) {
    curr_size = write(fd[1], "Hello, world!", 1);
    if (curr_size < 1) {
      printf("Pipe size: %d\n(число - степень двойки)\n", sum_size);
      break;
    }
    sum_size += curr_size;
  }

  close(fd[0]);
  close(fd[1]);

  return 0;
}
