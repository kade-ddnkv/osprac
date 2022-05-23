#define _XOPEN_SOURCE 500

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <ftw.h>
#include <unistd.h>

// Директория for_symlinks автоматически удаляется.
int unlink_cd(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
  int rv = remove(fpath);
  if (rv)
    perror(fpath);
  return rv;
}

int rmrf(char *path) {
  return nftw(path, unlink_cd, 64, FTW_DEPTH | FTW_PHYS);
}

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
      break;
    }
    fd = open(pathfrom, O_RDONLY);
    readretval = read(fd, string, 3);
    close(fd);
    if (readretval == -1) {
      printf("error on read, max rec depth=%d\n", depth);
      break;
    }
    if (strcmp(string, "ok") != 0) {
      printf("error on string in file, max rec depth=%d\n", depth);
      break;
    }
    depth++;
  }

  if (rmrf("for_symlinks") != -1) {
    printf("for_symlinks dir removed correctly\n");
  } else {
    printf("error while removing dir for_symlinks\n");
  }
  
  return 0;
}
