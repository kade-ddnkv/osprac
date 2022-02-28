#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
  char    *shm_array;
  int     shmid;
  char    pathname[] = "task7-1w.c";
  key_t   key;

  key = ftok(pathname,0);

  if ((shmid = shmget(key, 0, 0)) < 0) {
    printf("Can\'t find shared memory. Run task7-1w.c first.\n");
    exit(-1);
  }

  shm_array = (char *)shmat(shmid, NULL, 0);

  printf("%s", shm_array);

  shmdt(shm_array);

  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
