#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
  char    *shm_array;
  int     shmid;
  char    pathname[] = "task7-1w.c";
  key_t   key;
  int fd;

  key = ftok(pathname,0);

  if ((shmid = shmget(key, 3000, 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
      printf("Shared memory already exists, now run task7-1r.c\n");
      exit(-1);
    }
  }

  shm_array = (char *)shmat(shmid, NULL, 0);

  fd = open("task7-1w.c", O_RDONLY);
  // Это запишет в разделяемую память весь файл, без мусора в конце. 
  read(fd, shm_array, 3000);

  shmdt(shm_array);

  return 0;
}
