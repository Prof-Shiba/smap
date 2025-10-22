#pragma once

#include "../shiba-core/shiba.h"
#include <stdio.h>

#if defined _WIN32
  #include <Windows.h>
#else
  #include <pthread.h>
#endif

#if defined _WIN32
  typedef struct shiba_threads_win_convert_t {
    void* arg;
    void* (*function) (void*);
    uintptr_t result;
  }shiba_threads_win_convert_t;

  static DWORD WINAPI shiba_threads_windows_thread_create(LPVOID param) {
    shiba_threads_win_convert_t* win = (shiba_threads_win_convert_t*)param;
    void* res = win->function(win->arg);
    win->result = (uintptr_t)res;

    return 0;
  }
#endif

typedef struct shiba_threads_thread_t {
#if defined _WIN32
  HANDLE thread;
  shiba_threads_win_convert_t* result_context;
#else
  pthread_t thread;
#endif
} shiba_threads_thread_t;

typedef struct shiba_threads_mutex_t {
#if defined _WIN32
  HANDLE mutex;
#else
  pthread_mutex_t mutex;
#endif
} shiba_threads_mutex_t;

// Creates a thread, returns 0 on success
uint32 shiba_threads_thread_create(shiba_threads_thread_t* handle, void* (*function) (void*), void* arg);
// Join thread, returns 0 on success, also cleansup
uint32 shiba_threads_thread_join(shiba_threads_thread_t* handle, void** retval);

// init a mutex, returns 0 on success
uint32 shiba_threads_mutex_init(shiba_threads_mutex_t* handle);
// lock a mutex, returns 0 on success
uint32 shiba_threads_mutex_lock(shiba_threads_mutex_t* handle);
// unlock a mutex, returns 0 on success
uint32 shiba_threads_mutex_unlock(shiba_threads_mutex_t* handle);
// destroy a mutex, returns 0 on success. shouldn't fail unless you passed an invalid mutex
uint32 shiba_threads_mutex_destroy(shiba_threads_mutex_t* handle);
