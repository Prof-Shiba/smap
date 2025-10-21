#include "./shiba-threads.h"

uint32 shiba_threads_thread_create(shiba_threads_thread_t* thread, void* (*function) (void*), void* arg) {
  // TODO:
}

uint32 shiba_threads_thread_join(shiba_threads_thread_t* thread, void** retval) {
  // TODO:
}

uint32 shiba_threads_thread_destroy(shiba_threads_thread_t* thread) {
  // TODO:
}

uint32 shiba_threads_mutex_init(shiba_threads_mutex_t* mutex) {
  // TODO:
}

uint32 shiba_threads_mutex_lock(shiba_threads_mutex_t* mutex) {
  // TODO:
}

uint32 shiba_threads_mutex_unlock(shiba_threads_mutex_t* mutex) {
  // TODO:
}

uint32 shiba_threads_mutex_destroy(shiba_threads_mutex_t* mutex) {
  // TODO:
}
