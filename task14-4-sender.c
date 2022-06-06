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

int signal_i;
int32_t number_to_send;
int receiver_pid;

void send_next_bit() {
  signal_i++;
  int bit = number_to_send & (~0x7fffffff);
  number_to_send <<= 1;
  if (bit == 0) {
    kill(receiver_pid, SIGUSR1);
  } else {
    kill(receiver_pid, SIGUSR2);
  }
  if (signal_i == 32) {
    exit(0);
  }
}

void my_handler(int nsig) {
  if (nsig == SIGFPE) {
    kill(receiver_pid, SIGFPE);
  }
  if (nsig == SIGUSR2) {
    send_next_bit();
  }
}

int main(void) {
  pid_t pid = getpid();
  (void) signal(SIGFPE, SIG_IGN);

  printf("my pid: %d\n", pid);
  printf("enter receiver pid:\n");
  scanf("%d", &receiver_pid);
  printf("enter number to send:\n");
  scanf("%d", &number_to_send);
  (void) signal(SIGFPE, my_handler);
  (void) signal(SIGUSR1, my_handler);
  (void) signal(SIGUSR2, my_handler);

  while(1);
  return 0;
}
