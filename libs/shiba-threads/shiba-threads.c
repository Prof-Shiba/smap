#include "./shiba-threads.h"

uint32 shiba_threads_thread_create(shiba_threads_thread_t* handle, void* (*function) (void*), void* arg) {
  if (!handle) return 2;
  #if defined _WIN32
    shiba_threads_win_convert_t* win = malloc(sizeof(*win));
    if (!win) return 3;

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
  if (!handle) return 2;
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
  if (!handle) return 2;
  #if defined _WIN32
  // im defaulting to these params to make it consistent across linux and windows
  handle->mutex = CreateMutex(NULL, FALSE, NULL);
  if (!handle->mutex) return 1;

  return 0;
  #else
    return pthread_mutex_init(&handle->mutex, NULL); // this function always returns 0
  #endif
}

uint32 shiba_threads_mutex_lock(shiba_threads_mutex_t* handle) {
  if (!handle) return 2;
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
  if (!handle) return 2;
  #if defined _WIN32
    if (!ReleaseMutex(handle->mutex)) return 1;
    return 0;
  #else
    if (pthread_mutex_unlock(&handle->mutex) != 0) return 1;
    return 0;
  #endif
}

uint32 shiba_threads_mutex_destroy(shiba_threads_mutex_t* handle) {
  if (!handle) return 2;
  #if defined _WIN32
    if (!CloseHandle(handle->mutex)) return 1;
    return 0;
  #else
    if (pthread_mutex_destroy(&handle->mutex) != 0) return 1;
    return 0;
  #endif
}

shiba_threads_semaphore_t* shiba_threads_semaphore_init(const char* name, uint32 init_value) {
  shiba_threads_semaphore_t* handle = malloc(sizeof(*handle));
  if (!handle) return NULL;
  #if defined _WIN32
    handle->sem = CreateSemaphore(NULL, init_value, init_value, name);
    if (handle->sem == ERROR_ALREADY_EXISTS || handle->sem == NULL) {
      free(handle);
      return NULL;
    }
    return handle;
  #else
    if (name == NULL) {
      if (sem_init(&handle->sem, 0, init_value) != 0) {
        free(handle);
        return NULL;
      }
    }
    else {
      int len_name = strlen(name) + 2; // we need to append a '/' to the string
      char new_name[len_name];
      new_name[0] = '/';

      for (int i = 1; i < len_name - 1; i++)
        new_name[i] = name[i - 1];

      new_name[len_name - 1] = '\0';
      sem_unlink(new_name); // in case we crashed and this name is in memory
  
      sem_t* sem_ptr = sem_open(new_name, O_CREAT, 0644, init_value);
      if (sem_ptr == SEM_FAILED) {
        free(handle);
        return NULL;
      }
      handle->sem = *sem_ptr;
    }
    return handle;
  #endif
}

uint32 shiba_threads_semaphore_post(shiba_threads_semaphore_t* handle) {
  if (!handle) return 2;
  #if defined _WIN32
  if (ReleaseSemaphore(handle->sem, 1, NULL) == 0) return 1;
  return 0;
  #else
    if (sem_post(&handle->sem) != 0) return 1;
    return 0;
  #endif
}

uint32 shiba_threads_semaphore_wait(shiba_threads_semaphore_t* handle) {
  if (!handle) return 2;
  #if defined _WIN32
    DWORD ret = WaitForSingleObject(handle->sem, INFINITE);
    if (ret != WAIT_OBJECT_0) return 1;
    return 0;
  #else
    if (sem_wait(&handle->sem) != 0) return 1;
    return 0;
  #endif
}

uint32 shiba_threads_semaphore_destroy(shiba_threads_semaphore_t* handle) {
  if (!handle) return 2;
  #if defined _WIN32
    if (!CloseHandle(handle->sem)) {
      free(handle);
      return 1;
    }

    free(handle);
    return 0;
  #else
    if (sem_destroy(&handle->sem) != 0)
        return 1;
    return 0;
  #endif
}
