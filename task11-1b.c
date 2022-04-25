#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
  int msqid;
  char pathname[]="task11-1a.c";
  key_t  key;
  int len, maxlen;

  struct mymsgbuf
  {
    long mtype;
    struct {
      char header[128];
      char body[256];
    } msg;
  } mybuf;

  key = ftok(pathname,0);

  msqid = msgget(key, 0666 | IPC_CREAT);

  while (1) {
    maxlen = 512;

    msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0);

    if (mybuf.mtype == LAST_MESSAGE) {
      msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
      exit(0);
    }

    printf("message type = %ld, header = %s, body = %s\n", mybuf.mtype, mybuf.msg.header, mybuf.msg.body);
  }

  return 0;
}
