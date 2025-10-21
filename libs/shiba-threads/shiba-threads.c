#include "./shiba-threads.h"

uint32 shiba_threads_thread_create(shiba_threads_thread_t* handle, void* (*function) (void*), void* arg) {
  // TODO:
}

uint32 shiba_threads_thread_join(shiba_threads_thread_t* handle, void** retval) {
  #if defined _WIN32
    DWORD ret = WaitForSingleObject(handle->thread, INFINITE);
    if (ret != WAIT_OBJECT_0) return 1;

    if (retval) 
      *retval = NULL; // WARN: im just going to ignore it on windows for now...ill implement it if i need later

    return 0;
  #else
    // linux
  #endif
}

void shiba_threads_thread_destroy(shiba_threads_thread_t* handle) {
  #if defined _WIN32
    ExitThread(handle->thread);
  #else
    // linux
  #endif
}

uint32 shiba_threads_mutex_init(shiba_threads_mutex_t* handle) {
  #if defined _WIN32
  // im defaulting to these params to make it consistent across linux and windows
  handle->mutex = CreateMutex(NULL, FALSE, NULL);
  if (!handle->mutex) return 1;

  return 0;
  #else
    // linux
  #endif
}

uint32 shiba_threads_mutex_lock(shiba_threads_mutex_t* handle) {
  #if defined _WIN32
  DWORD ret = WaitForSingleObject(handle->mutex, INFINITE);
  if (ret != WAIT_OBJECT_0) return 1;
  return 0;
  #else
    // linux
  #endif
}

uint32 shiba_threads_mutex_unlock(shiba_threads_mutex_t* handle) {
  #if defined _WIN32
    if (!ReleaseMutex(handle->mutex)) return 1;
    return 0;
  #else
    // linux
  #endif
}

uint32 shiba_threads_mutex_destroy(shiba_threads_mutex_t* handle) {
  #if defined _WIN32
    if (!CloseHandle(handle->mutex)) return 1;
    return 0;
  #else
    // linux
  #endif
}
