#include "./shiba-threads.h"

uint32 shiba_threads_thread_create(shiba_threads_thread_t* handle, void* (*function) (void*), void* arg) {
  #if defined _WIN32
    shiba_threads_win_convert_t* win = malloc(sizeof(*win));
    if (!win) return 1;

    win->arg = arg;
    win->function = function;
    win->result = 0;

    handle->result_context = win;
    handle->thread = CreateThread(NULL, 0, shiba_threads_windows_thread_create, win, 0, NULL);

    if (handle->thread == NULL) {
      free(win);
      handle->result_context = NULL;
      return 1;
    }

    return 0;
  #else
    if (pthread_create(&handle->thread, NULL, function, arg) != 0)
        return 1;
    return 0;
  #endif
}

uint32 shiba_threads_thread_join(shiba_threads_thread_t* handle, void** retval) {
  #if defined _WIN32
    DWORD ret = WaitForSingleObject(handle->thread, INFINITE);
    if (ret != WAIT_OBJECT_0) return 1;

    if (retval) 
      *retval = (void*)handle->result_context->result;

    free(handle->result_context);
    handle->result_context = NULL;

    CloseHandle(handle->thread);
    handle->thread = NULL;

    return 0;
  #else
    if (pthread_join(handle->thread, retval) != 0) return 1;
    return 0;
  #endif
}

uint32 shiba_threads_mutex_init(shiba_threads_mutex_t* handle) {
  #if defined _WIN32
  // im defaulting to these params to make it consistent across linux and windows
  handle->mutex = CreateMutex(NULL, FALSE, NULL);
  if (!handle->mutex) return 1;

  return 0;
  #else
    pthread_mutex_init(&handle->mutex, NULL); // this function always returns 0
    return 0;
  #endif
}

uint32 shiba_threads_mutex_lock(shiba_threads_mutex_t* handle) {
  #if defined _WIN32
  DWORD ret = WaitForSingleObject(handle->mutex, INFINITE);
  if (ret != WAIT_OBJECT_0) return 1;
  return 0;
  #else
    if (pthread_mutex_lock(&handle->mutex) != 0) return 1;
    return 0;
  #endif
}

uint32 shiba_threads_mutex_unlock(shiba_threads_mutex_t* handle) {
  #if defined _WIN32
    if (!ReleaseMutex(handle->mutex)) return 1;
    return 0;
  #else
    // linux
    if (pthread_mutex_unlock(&handle->mutex) != 0) return 1;
    return 0;
  #endif
}

uint32 shiba_threads_mutex_destroy(shiba_threads_mutex_t* handle) {
  #if defined _WIN32
    if (!CloseHandle(handle->mutex)) return 1;
    return 0;
  #else
    // linux
    if (pthread_mutex_destroy(&handle->mutex) != 0) return 1;
    return 0;
  #endif
}
