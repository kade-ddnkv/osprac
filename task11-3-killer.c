#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int msqid;
  char pathname[]="task11-3-server.c";
  key_t  key;
  int len;

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

  // Посылает сообщение для остановки сервера.
  mybuf.mtype = 1;
  len = 0;
  msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0);

  return 0;
}
