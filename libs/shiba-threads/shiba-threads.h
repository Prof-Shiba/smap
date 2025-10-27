#pragma once

#include "../shiba-core/shiba.h"
#include <stdio.h>
#include <stdlib.h>

#if defined _WIN32
  #include <Windows.h>
#else
  #include <pthread.h>
  #include <semaphore.h>
#endif

#if defined _WIN32
  typedef struct shiba_threads_win_convert_t {
    void* arg;
    void* (*function) (void*);
    uintptr_t result; // shoutout to zorky for helping with this :)
  }shiba_threads_win_convert_t;

  static DWORD WINAPI shiba_threads_windows_thread_create(LPVOID param) {
    shiba_threads_win_convert_t* win = (shiba_threads_win_convert_t*)param;
    void* res = win->function(win->arg);
    win->result = (uintptr_t)res;

    return 0;
  }
#endif

// THREAD TYPE
typedef struct shiba_threads_thread_t {
#if defined _WIN32
  HANDLE thread;
  shiba_threads_win_convert_t* result_context;
#else
  pthread_t thread;
#endif
} shiba_threads_thread_t;

// MUTEX TYPE
typedef struct shiba_threads_mutex_t {
#if defined _WIN32
  HANDLE mutex;
#else
  pthread_mutex_t mutex;
#endif
} shiba_threads_mutex_t;

// SEMAPHORE TYPE
typedef struct shiba_threads_semaphore_t {
#if defined _WIN32
  HANDLE sem;
#else
  sem_t sem;
#endif
} shiba_threads_semaphore_t;

// Creates a thread, returns 0 on success, 1 on fail, 2 on handle invalid
uint32 shiba_threads_thread_create(shiba_threads_thread_t* handle, void* (*function) (void*), void* arg);
// Join thread, returns 0 on success, 1 on fail, 2 on handle invalid. also cleansup
uint32 shiba_threads_thread_join(shiba_threads_thread_t* handle, void** retval);

// init a mutex, returns 0 on success, 1 on fail, 2 on handle invalid
uint32 shiba_threads_mutex_init(shiba_threads_mutex_t* handle);
// lock a mutex, returns 0 on success, 1 on fail, 2 on handle invalid
uint32 shiba_threads_mutex_lock(shiba_threads_mutex_t* handle);
// unlock a mutex, returns 0 on success, 1 on fail, 2 on handle invalid
uint32 shiba_threads_mutex_unlock(shiba_threads_mutex_t* handle);
// destroy a mutex, returns 0 on success. shouldn't fail unless you passed an invalid mutex or handle
uint32 shiba_threads_mutex_destroy(shiba_threads_mutex_t* handle);

// creates a semaphore. Returns a semaphore on success, NULL on fail.
shiba_threads_semaphore_t* shiba_threads_semaphore_init(const char* name, uint32 init_value);
// locks the semaphore pointed to by handle. returns 0 on success, 1 on fail, 2 on handle invalid
uint32 shiba_threads_semaphore_wait(shiba_threads_semaphore_t* handle);
// unlocks semaphore pointed to by handle. returns 0 on success, 1 on fail, 2 on handle invalid
uint32 shiba_threads_semaphore_post(shiba_threads_semaphore_t* handle);
// Takes a semaphore handle. returns 0 on success, 1 on failure, 2 if handle invalid.
uint32 shiba_threads_semaphore_destroy(shiba_threads_semaphore_t* handle);
