#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Алгоритм:

// Первоначальная инициализация семафора:
// sem = 1

// цикл в parent:
// write
// D
// D
// read

// после цикла в parent (чтобы дождаться завершения ребенка):
// D2

// цикл в child:
// Z
// read
// write
// A2

// после цикла в child (чтобы дождаться завершения ребенка):
// A

int main()
{
  int     fd[2], result;

  size_t size;
  char  resstring[14];

  char pathname[] = "task9-3.c";
  key_t key;
  int semid;
  struct sembuf mybuf;

  // Первоначальная инициализация.

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }

  mybuf.sem_num = 0;
  mybuf.sem_op  = 1;
  mybuf.sem_flg = 0;

  if (semop(semid, &mybuf, 1) < 0) {
    printf("Can\'t add 1 to semaphore\n");
    exit(-1);
  }

  printf("input n (number of iterations > 0):\n");
  int n;
  scanf("%d", &n);

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }

  result = fork();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

   /* Parent process */

    for (int i = 0; i < n; ++i) {

      // write
      size = write(fd[1], "Hello, world!", 14);
      printf("write parent, i=%d\n", i);

      // D D
      mybuf.sem_op  = -1;
      semop(semid, &mybuf, 1);
      semop(semid, &mybuf, 1);

      // read
      size = read(fd[0], resstring, 14);
      printf("read parent, i=%d\n", i);
    }

    close(fd[0]);
    close(fd[1]);

    // Чтобы родитель дождался завершения ребенка.
    mybuf.sem_op  = -2;
    semop(semid, &mybuf, 1);

    printf("Parent exit\n");

  } else {

    /* Child process */

    for (int i = 0; i < n; ++i) {

      // Z
      mybuf.sem_op  = 0;
      semop(semid, &mybuf, 1);

      // read
      size = read(fd[0], resstring, 14);
      printf("read child, i=%d\n", i);

      // write
      size = write(fd[1], "Hello, world!", 14);
      printf("write child, i=%d\n", i);

      // A2
      mybuf.sem_op  = 2;
      semop(semid, &mybuf, 1);
    }

    close(fd[0]);
    close(fd[1]);

    printf("Child exit\n");

    // Чтобы родитель дождался завершения ребенка.
    mybuf.sem_op  = 1;
    semop(semid, &mybuf, 1);
  }

  return 0;
}
