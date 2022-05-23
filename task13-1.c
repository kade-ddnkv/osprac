#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

// Директория for_symlinks автоматически не удаляется.

int main()
{
  int     fd;
  size_t  size;
  char    string[] = "ok";
  char    pathfrom[512];
  char    pathto[512];

  struct stat st = {0};
  if (stat("for_symlinks", &st) == -1) {
    mkdir("for_symlinks", S_IRWXU);
  }

  fd = open("for_symlinks/0", O_RDWR | O_CREAT, 0777);
  write(fd, string, 3);
  close(fd);

  int depth = 0;
  int readretval;
  while (1) {
    sprintf(pathto, "%d", depth);
    sprintf(pathfrom, "for_symlinks/%d", depth+1);
    if (symlink(pathto, pathfrom) == -1) {
      printf("error on symlink, max rec depth=%d\n", depth);
      exit(0);
    }
    fd = open(pathfrom, O_RDONLY);
    readretval = read(fd, string, 3);
    close(fd);
    if (readretval == -1) {
      printf("error on read, max rec depth=%d\n", depth);
      exit(0);
    }
    if (strcmp(string, "ok") != 0) {
      printf("error on string in file, max rec depth=%d\n", depth);
      exit(0);
    }
    depth++;
  }
  return 0;
}
