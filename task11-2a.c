#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Я не уверен, сделал ли я полноценный двусторонним обмен сообщениями.
// В двустороннем общении непонятно, кому и при каких условиях удалять очередь сообщений. Поэтому я ее не удаляю.

int main(void)
{
  int msqid;
  char pathname[]="task11-1a.c";
  key_t  key;
  int i, len, maxlen;

  struct mymsgbuf
  {
    long mtype;
    struct {
      bool is_final_msg;
      char header[128];
      char body[256];
    } msg;
  } mybuf;

  key = ftok(pathname,0);

  msqid = msgget(key, 0666 | IPC_CREAT);

  // Посылает разом 5 сообщений с типом 1.
  for (i = 1; i <= 5; i++) {
    mybuf.mtype = 1;
    mybuf.msg.is_final_msg = false;
    sprintf(mybuf.msg.header, "H#%d", i);
    sprintf(mybuf.msg.body, "B#%d", i);
    len = sizeof(mybuf.msg);

    msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0);
  }

  // Последнее сообщение.
  mybuf.mtype = 1;
  len         = sizeof(mybuf.msg);
  mybuf.msg.is_final_msg = true;
  msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0);

  // Принимает все сообщения с типом 2 до последнего.
  while (1) {
    maxlen = 512;

    msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 2, 0);

    if (mybuf.msg.is_final_msg == true) {
      printf("first program exit\n");
      exit(0);
    }

    printf("message type = %ld, is_final = %s, header = %s, body = %s\n", mybuf.mtype, mybuf.msg.is_final_msg ? "true" : "false", mybuf.msg.header, mybuf.msg.body);
  }

  return 0;
}
