#pragma once

#include "../shiba-core/shiba.h"

#if defined _WIN32
  #include <Windows.h>
#else
  #include <pthread.h>
#endif

typedef struct shiba_threads_thread_t {
#if defined _WIN32
  HANDLE thread;
  DWORD thread_id; // might be able to see which threads get blocked or have issues
#else
  pthread_t thread;
#endif
} shiba_threads_thread_t;

// TODO: Shiba_thread create func, shiba_thread_join, shiba_thread_close, etc
// Read this: https://web.archive.org/web/20121023005749/http://www.cs.rpi.edu/academics/courses/netprog/WindowsThreads.html 
