#include "./libs/shiba-core/shiba.h"
#include "./libs/shiba-network/shiba-network.h"
#include "./libs/shiba-threads/shiba-threads.h"

#define THREAD_NUM 16

shiba_threads_semaphore_t* semaphore;

void* routine(void* args) {
    printf("(%d) Waiting in the login queue\n", *(int*)args);
    if (shiba_threads_semaphore_wait(semaphore) != 0) {
        fprintf(stderr, "Semaphore wait failed!\n");
        return NULL;
    }

    printf("(%d) Logged in\n", *(int*)args);
    #ifdef _WIN32
        Sleep(rand() % 5 + 1);
    #else
        sleep(rand() % 5 + 1);
    #endif
    printf("(%d) Logged out\n", *(int*)args);

    shiba_threads_semaphore_post(semaphore);
    free(args);
    return NULL;
}

int main(int argc, char *argv[]) {
    shiba_threads_thread_t th[THREAD_NUM];
    semaphore = shiba_threads_semaphore_init("Sheeeeba!!!!!", 32);

    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        int* a = malloc(sizeof(*a));
        *a = i;
        if (shiba_threads_thread_create(&th[i], &routine, a) != 0) {
          fprintf(stderr, "Failed to create thread!\n");
          return 1;
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (shiba_threads_thread_join(&th[i], NULL) != 0) {
          fprintf(stderr, "Failed to join thread!\n");
          return 1;
        }
    }

    shiba_threads_semaphore_destroy(semaphore);
    return 0;
}
