#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SHIBA_MEM_DEBUG_OFF
#include "shiba.h"

#define SHIBA_MEM_OVER_ALLOC 32
#define SHIBA_MEM_MAGIC_NUM 132

extern void shiba_memory_debug_print(uint min_allocations);

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

boolean shiba_memory_debug() {
  boolean output = FALSE;
  uint k;
  
  if (shiba_alloc_mutex)
    shiba_alloc_mutex_lock(shiba_alloc_mutex);

  for (int i = 0; i < shiba_alloc_line_count; i++) {
    for (int j = 0; j < shiba_alloc_lines[i].alloc_count; j++)
    {
      uint8* buf = shiba_alloc_lines[i].allocs[j].buffer;
      uint size = shiba_alloc_lines[i].allocs[j].size;

      for (k = 0; k < SHIBA_MEM_OVER_ALLOC; k++)
        if (buf[size + k] != SHIBA_MEM_MAGIC_NUM)
          break;

      if (k < SHIBA_MEM_OVER_ALLOC) {
        fprintf(stderr, "MEMORY ERROR: We wrote past the end of our allocated buffer at line %u in file %s!\n", shiba_alloc_lines[i].line, shiba_alloc_lines[i].file);

        // kill it
        {
          uint* pay_attention_bro = NULL;
          pay_attention_bro[0] = 69;
        }

        output = TRUE;
      }
    }
  }

  if (shiba_alloc_mutex)
    shiba_alloc_mutex_unlock(shiba_alloc_mutex);

  return output;
}

void shiba_memory_debug_add(void* ptr, uint size, char* file, uint line) {
  uint i, j;

  for (i = 0; i < SHIBA_MEM_OVER_ALLOC; i++)
    ((uint8*) ptr)[size + i] = SHIBA_MEM_MAGIC_NUM;

  for (i = 0; i < shiba_alloc_line_count; i++) {
    if (line == shiba_alloc_lines[i].line)
    {
      for (j = 0; file[j] != 0 && file[j] == shiba_alloc_lines[i].file[j]; j++);
      if (file[j] == shiba_alloc_lines[i].file[j])
        break;
    }
  }

  if (i < shiba_alloc_line_count) {
    if (shiba_alloc_lines[i].alloc_allocated == shiba_alloc_lines[i].alloc_count)
    {
      shiba_alloc_lines[i].alloc_allocated += 1024;
      shiba_alloc_lines[i].allocs = realloc(shiba_alloc_lines[i].allocs, (sizeof *shiba_alloc_lines[i].allocs) * shiba_alloc_lines[i].alloc_allocated);
    }

    shiba_alloc_lines[i].allocs[shiba_alloc_lines[i].alloc_count].size = size;
    shiba_alloc_lines[i].allocs[shiba_alloc_lines[i].alloc_count++].buffer = ptr;
    shiba_alloc_lines[i].size += size;
    shiba_alloc_lines[i].allocated++;
  }
  else {
    if (i < 1024) {
      shiba_alloc_lines[i].line = line;
      for (j = 0; j < 255 && file[j] != 0; j++)
        shiba_alloc_lines[i].file[j] = file[j];

      shiba_alloc_lines[i].file[j] = 0;
      shiba_alloc_lines[i].alloc_allocated = 256;
      shiba_alloc_lines[i].allocs = malloc((sizeof *shiba_alloc_lines[i].allocs) * shiba_alloc_lines[i].alloc_allocated);
      shiba_alloc_lines[i].allocs[0].size = size;
      shiba_alloc_lines[i].allocs[0].buffer = ptr;
      shiba_alloc_lines[i].alloc_count = 1;
      shiba_alloc_lines[i].size = size;
      shiba_alloc_lines[i].freed = 0;
      shiba_alloc_lines[i].allocated++;
      shiba_alloc_line_count++;
    }
  }
}

void* shiba_memory_debug_malloc(uint size, char* file, uint line) {
  void* ptr;

  if (shiba_alloc_mutex != NULL)
    shiba_alloc_mutex_lock(shiba_alloc_mutex);

  ptr = malloc(size + SHIBA_MEM_OVER_ALLOC);

  if (!ptr) {
    fprintf(stderr, "MEMORY ERROR: Malloc returned [NULL] when trying to allocate %u bytes at line %u in file %s!\n", size, line, file);

    if (shiba_alloc_mutex)
      shiba_alloc_mutex_unlock(shiba_alloc_mutex);

    shiba_memory_debug_print(0);
    exit(0);
  }

  for (uint i = 0; i < size + SHIBA_MEM_OVER_ALLOC; i++)
    ((uint8* ) ptr)[i] = SHIBA_MEM_MAGIC_NUM + 1;

  shiba_memory_debug_add(ptr, size, file, line);

  if (shiba_alloc_mutex)
    shiba_alloc_mutex_unlock(shiba_alloc_mutex);

  return ptr;
}

void* shiba_memory_debug_realloc(void* ptr, uint size, char* file, uint line) {
  // TODO:
}

boolean shiba_memory_debug_remove(void* buf) {
  for (int i = 0; i < shiba_alloc_line_count; i++) {
    for (int j = 0; j < shiba_alloc_lines[i].alloc_count; j++)
    {
      if (shiba_alloc_lines[i].allocs[j].buffer == buf) {
        for (int k = 0; k < SHIBA_MEM_OVER_ALLOC; k++)
        {
          if (((uint8* ) buf)[shiba_alloc_lines[i].allocs[j].size + k] != SHIBA_MEM_MAGIC_NUM)
            break;

          if (k < SHIBA_MEM_OVER_ALLOC)
            fprintf(stderr, "MEMORY ERROR: Wrote past allocated buffer at line %u in file %s!\n", shiba_alloc_lines[i].line, shiba_alloc_lines[i].file);

          shiba_alloc_lines[i].size -= shiba_alloc_lines[i].allocs[j].size;
          shiba_alloc_lines[i].allocs[j] = shiba_alloc_lines[i].allocs[--shiba_alloc_lines[i].alloc_count];
          shiba_alloc_lines[i].freed++;

          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

void shiba_memory_debug_free(void* buffer) {
  if (shiba_alloc_mutex)
    shiba_alloc_mutex_lock(shiba_alloc_mutex);
}

void shiba_memory_debug_print(uint min_allocations) {
  // TODO:
}

void shiba_memory_debug_reset() {
  // TODO:
}
