#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

// Алгоритм такой:
// сначала устанавливается соединение между процессами:
// receiver начинает спамить сигналом SIGFPE sender-у, чтобы сказать, что он готов принимать.
// при получении SIGFPE sender отправляет SIGFPE receiver-у, чтобы тот успокоился и понял, что соединение установлено.
// дальше каждый раз receiever отправляет SIGUSR2 sender-у, показывая, что готов принимать следующий бит.
// sender сам ничего не делает, только отзывается на запросы receiver-а SIGUSR2.

int signal_i = 0;
int32_t received_number = 0;
int sender_pid;
int stop_notifying_sender = 0;

void my_handler(int nsig) {
  int bit;
  if (nsig == SIGFPE) {
    stop_notifying_sender = 1;
  }
  if (nsig == SIGUSR1 || nsig == SIGUSR2) {
    signal_i++;
    if (nsig == SIGUSR1) {
      bit = 0;
    } else {
      bit = 1;
    }
    received_number <<= 1;
    received_number |= bit;
    if (signal_i == 32) {
      printf("received number: %d\n", received_number);
      exit(0);
    }
    kill(sender_pid, SIGUSR2);
  }
}

int main(void) {
  pid_t pid = getpid();

  printf("my pid: %d\n", pid);
  printf("enter receiver pid:\n");
  scanf("%d", &sender_pid);
  (void) signal(SIGFPE, my_handler);
  (void) signal(SIGUSR1, my_handler);
  (void) signal(SIGUSR2, my_handler);
  while (1) {
    if (stop_notifying_sender) {
      kill(sender_pid, SIGUSR2);
      break;
    }
    kill(sender_pid, SIGFPE);
  }

  while(1);
  return 0;
}
