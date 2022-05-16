#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
  int msqid, mypid;
  char pathname[]="task11-3-server.c";
  key_t  key;
  int len, maxlen;

  maxlen = 512;

  struct mymsgbuf
  {
    long mtype;
    struct {
      int client_id;
      double number;
    } body;
  } mybuf;

  key = ftok(pathname,0);

  msqid = msgget(key, 0666);
  mypid = getpid();

  // Отправка реквеста серверу.
  mybuf.mtype = 2;
  mybuf.body.client_id = mypid;
  printf("input request number: ");
  scanf("%lf", &mybuf.body.number);
  len = sizeof(mybuf.body);
  msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0);

  // Получение ответа.
  msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, mypid, 0);
  printf("responce number: %lf\n", mybuf.body.number);

  return 0;
}
