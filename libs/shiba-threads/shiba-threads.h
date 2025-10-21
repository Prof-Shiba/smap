#pragma once

#include "../shiba-core/shiba.h"
#include <stdio.h>

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

// NOTE: windows wants DWORD WINAPI ThreadFunc(LPVOID lpParam) where DWORD = int32 and LPVOID is just a void*
// WINAPI should just be like __stdcall or something

/*
DWORD WINAPI function(LPVOID lpParam) {
  printf("some text\n");
  return 0;
}

this needs to be converted^
*/
uint32 shiba_threads_thread_create(shiba_threads_thread_t* handle, void* (*function) (void*), void* arg);
uint32 shiba_threads_thread_join(shiba_threads_thread_t* handle, void** retval);
uint32 shiba_threads_thread_destroy(shiba_threads_thread_t* handle);

uint32 shiba_threads_mutex_init(shiba_threads_mutex_t* handle);
uint32 shiba_threads_mutex_lock(shiba_threads_mutex_t* handle);
uint32 shiba_threads_mutex_unlock(shiba_threads_mutex_t* handle);
uint32 shiba_threads_mutex_destroy(shiba_threads_mutex_t* handle);

// TODO: Shiba_thread create func, shiba_thread_join, shiba_thread_close, etc
// Read this: https://web.archive.org/web/20121023005749/http://www.cs.rpi.edu/academics/courses/netprog/WindowsThreads.html 

/*
windows equivalent to pthread_create:

HANDLE CreateThread(
  LPSECURITY_ATTRIBUTES lpThreadAttributes,  // pointer to security attributes - SET TO NULL
  DWORD dwStackSize,                         // initial thread stack size - USE 0
  LPTHREAD_START_ROUTINE lpStartAddress,     // pointer to thread function
  LPVOID lpParameter,                        // argument for new thread
  DWORD dwCreationFlags,                     // creation flags - USE 0
  LPDWORD lpThreadId                         // pointer to receive thread ID - NULL
);

example:
HANDLE CreateThread(NULL, 0, &func_here, &arg_to_func, 0, &ptr_to_thread_id); // returns a HANDLE if success, NULL if fail

windows equivalent to pthread_join:

DWORD WaitForSingleObject(
  HANDLE hHandle,        // handle to object to wait for
  DWORD dwMilliseconds   // time-out interval in milliseconds
);

ghMutex = CreateMutex( 
    NULL,              // default security attributes - USE NULL
    FALSE,             // initially not owned - USE FALSE
    NULL);             // unnamed mutex - USE NULL

if (ghMutex == NULL) 
{
    printf("CreateMutex error: %d\n", GetLastError());
    return 1;
}

CloseHandle(aThread[i]);
CloseHandle(ghMutex);

*/

