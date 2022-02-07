#include <stdio.h>

int main(int argc, char **argv, char **envp)
{
  printf("Arguments: ");
  int i = 0;
  while (argv[i] != NULL) {
    printf("%s", argv[i++]);
  }
  printf("\n");
  printf("Environment parameters:\n");
  i = 0;
  while (envp[i] != NULL) {
    printf("%s\n", envp[i++]);
  }

  return 0;
}
