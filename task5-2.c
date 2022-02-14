#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  // fd_up - от ребенка к родителю.
  // fd_down - от родителя к ребенку.
  int fd_up[2], fd_down[2], result;

  size_t size;
  char resstring[25];

  // Родитель отправляет сообщение ребенку, ждет ответа, выводит ответ.
  // Это происходит 2 раза.
  // У меня на компьютере выводится 2 раза подряд "Parent received", 
  // это, похоже, потому, что операция (действие с pipe + вывод сообщения) неатомарна.

  if (pipe(fd_up) < 0 || pipe(fd_down) < 0) {
    printf("Can\'t open pipes\n");
    exit(-1);
  }

  result = fork();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

    /* Parent process */

    for (int i = 0; i < 2; ++i) {
      close(fd_down[0]);
      write(fd_down[1], "message_to_child", 25);
      close(fd_down[1]);
    
      close(fd_up[1]);
      read(fd_up[0], resstring, 25);
      printf("Parent recieved message: %s\n", resstring);
      close(fd_up[0]);
    }

    printf("Parent exit.\n");

  } else {

    /* Child process */

    for (int i = 0; i < 2; ++i) {
      close(fd_down[1]);
      read(fd_down[0], resstring, 25);
      printf("Child recieved message: %s\n", resstring);
      close(fd_down[0]);

      close(fd_up[0]);
      write(fd_up[1], "message_to_parent", 25);
      close(fd_up[1]);
    }

    printf("Child exit.\n");
  }

  return 0;
}
