#include "./libs/shiba-core/shiba.h"
#include "./libs/shiba-network/shiba-network.h"
#include "./libs/shiba-threads/shiba-threads.h"
#include <stdio.h>

#define ARRAY_SIZE 20
shiba_threads_thread_t threads[ARRAY_SIZE];
shiba_threads_mutex_t mutex;
int j = 0;

void* function(void* arg) {
  shiba_threads_mutex_lock(&mutex);
  for (int i = 0; i < 100000; i++) {
    j++;
  }
  shiba_threads_mutex_unlock(&mutex);
  return NULL;
}

int main(int argc, char* argv[])
{
  printf("smap starting...\n\n");

  if (shiba_threads_mutex_init(&mutex) != 0) {
      fprintf(stderr, "Failed to create mutex!\n");
      return 1;
    }

  for (int i = 0; i < ARRAY_SIZE; i++) {
    if (shiba_threads_thread_create(threads + i, &function, NULL) != 0) {
      fprintf(stderr, "Failed to create thread number: %d!\n", i);
      return 1;
    }

    printf("Thread %d has started!\n", i);
  }
  printf("\n");

  for (int i = 0; i < ARRAY_SIZE; i++) {
    if (shiba_threads_thread_join(threads + i, NULL) != 0) {
      fprintf(stderr, "Failed to join thread number: %d!\n", i);
      return 1;
    }
    printf("Thread %d has been joined!\n", i);
  }
  printf("\n");

  if (shiba_threads_mutex_destroy(&mutex) != 0) {
    fprintf(stderr, "Failed to destroy mutex!\n");
    return 1;
  }

  printf("Value: %d\n", j);
  return 0;
}
