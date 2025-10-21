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

// TODO: shiba threads mutex init, mutex destroy

// TODO: Shiba_thread create func, shiba_thread_join, shiba_thread_close, etc
// Read this: https://web.archive.org/web/20121023005749/http://www.cs.rpi.edu/academics/courses/netprog/WindowsThreads.html 

/*
windows equivalent to pthread_create:

HANDLE CreateThread(
  LPSECURITY_ATTRIBUTES lpThreadAttributes,  // pointer to security attributes
  DWORD dwStackSize,                         // initial thread stack size
  LPTHREAD_START_ROUTINE lpStartAddress,     // pointer to thread function
  LPVOID lpParameter,                        // argument for new thread
  DWORD dwCreationFlags,                     // creation flags
  LPDWORD lpThreadId                         // pointer to receive thread ID
);

example:
HANDLE CreateThread(NULL, 0, &func_here, &arg_to_func, 0, &ptr_to_dword); // returns a HANDLE if success, NULL if fail

windows equivalent to pthread_join:

DWORD WaitForSingleObject(
  HANDLE hHandle,        // handle to object to wait for
  DWORD dwMilliseconds   // time-out interval in milliseconds
);

ghMutex = CreateMutex( 
    NULL,              // default security attributes
    FALSE,             // initially not owned
    NULL);             // unnamed mutex

if (ghMutex == NULL) 
{
    printf("CreateMutex error: %d\n", GetLastError());
    return 1;
}

*/

