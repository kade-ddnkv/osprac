#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a = 0;

void *subthread_func_1(void *dummy)
{
  pthread_t mythid;
  mythid = pthread_self();
  printf("Subthread %u created. It will increment a 50 million times.\n", mythid);
  for (int i = 0; i < 50000000; ++i) {
    a += 1;
  }
  return NULL;
}

void *subthread_func_2(void *dummy)
{
  pthread_t mythid;
  mythid = pthread_self();
  printf("Subthread %u created. It will increment a 100 million times.\n", mythid);
  for (int i = 0; i < 100000000; ++i) {
    a += 1;
  }
  return NULL;
}

int main()
{
  pthread_t thid_1, thid_2, mythid;
  pthread_create (&thid_1, (pthread_attr_t *)NULL, subthread_func_1, NULL);
  pthread_create (&thid_2, (pthread_attr_t *)NULL, subthread_func_2, NULL);

  mythid = pthread_self();
  printf("Main thread %u created\n", mythid);

  pthread_join(thid_1, (void **)NULL);
  pthread_join(thid_2, (void **)NULL);

  // Значение a после выполнения с большой вероятностью будет меньше 150 миллионов.
  printf("Main thread %u, after subthreads finished work, value of a = %d\n", mythid, a);

  return 0;
}
