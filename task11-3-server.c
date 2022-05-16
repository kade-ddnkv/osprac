#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

int main(void)
{
  int msqid;
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

  // Одновременно может существовать только один сервер.
  msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);
  if (errno == EEXIST) {
    printf("only one server instance can exist at one time\n");
    exit(0);
  }
  printf("server started\n");

  // Цикл по принятию реквестов.
  // mtype == 1 - сообщение для остановки сервера
  // mtype == 2 - обычное сообщение от клиента
  while (1) {
    msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, -2, 0);

    if (mybuf.mtype == 1) {
      break;
    }

    // Отправка респонса.
    printf("request from client with pid: %d\n", mybuf.body.client_id);
    mybuf.mtype = mybuf.body.client_id;
    mybuf.body.number *= mybuf.body.number;
    len = sizeof(mybuf.body);
    msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0);
  }

  msgctl(msqid, IPC_RMID, NULL);
  printf("server stopped, message queue deleted\n");

  return 0;
}
