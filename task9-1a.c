#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

// Операция D(S, 5)

int main(int argc, char *argv[], char *envp[])
{
  int    semid;
  char   pathname[]="task9-1a.c";
  key_t  key;
  struct sembuf mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }
  
  mybuf.sem_num = 0;
  mybuf.sem_op  = -5;
  mybuf.sem_flg = 0;

  if (semop(semid, &mybuf, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }

  printf("The condition is present\n");
  semctl(semid, 0, IPC_RMID);
  printf("Sem is deleted\n");
  return 0;
}
