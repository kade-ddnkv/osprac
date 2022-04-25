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
  int i,len;

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

  for (i = 1; i <= 5; i++) {
    mybuf.mtype = 1;
    sprintf(mybuf.msg.header, "H#%d", i);
    sprintf(mybuf.msg.body, "B#%d", i);
    len = sizeof(mybuf.msg);

    msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0);
  }

  mybuf.mtype = LAST_MESSAGE;
  len         = 0;

  msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0);

  return 0;
}
