#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SHIBA_MEM_DEBUG_OFF
#include "shiba.h"

#define SHIBA_MEM_OVER_ALLOC 32
#define SHIBA_MEM_MAGIC_NUM 132

typedef struct {
  uint size;
  void* buffer;
}STMemAllocBuf;

typedef struct {
  STMemAllocBuf* allocs;
  uint line;
  uint alloc_count;
  uint alloc_allocated;
  uint size;
  uint allocated;
  uint freed;
  char file[256];
}STMemAllocLine;

STMemAllocLine shiba_alloc_lines[1024];
uint shiba_alloc_line_count = 0;
void* shiba_alloc_mutex = NULL;
void (*shiba_alloc_mutex_lock)(void* mutex) = NULL;
void (*shiba_alloc_mutex_unlock)(void* mutex) = NULL;

void shiba_memory_debug_init(void (*lock)(void* mutex), void (*unlock)(void* mutex), void* mutex) {
  shiba_alloc_mutex = mutex;
  shiba_alloc_mutex_lock = lock;
  shiba_alloc_mutex_unlock = unlock;
}

void shiba_memory_debug_malloc(uint size, char* file, uint line) {
  // TODO:
}

void shiba_memory_debug_realloc(void* ptr, uint size, char* file, uint line) {
  // TODO:
}

void shiba_memory_debug_free(void* buffer) {
  // TODO:
}

void shiba_memory_debug_print(uint min_allocations) {
  // TODO:
}

void shiba_memory_debug_reset() {
  // TODO:
}

boolean shiba_memory_debug() {
  // TODO:
}
