#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SHIBA_MEM_DEBUG_OFF
#include "shiba.h"

#define SHIBA_MEM_OVER_ALLOC 32 // extra bytes, if someone writes into it, they overran their buffer
#define SHIBA_MEM_MAGIC_NUM 132 // this is the value we use to test if it was written into

extern void shiba_memory_debug_print(u32 min_allocations);

typedef struct {
  void* buffer;
  u32 size;
}ST_MemAllocBuf;

typedef struct {
  ST_MemAllocBuf* allocs;
  u32 line;
  u32 alloc_count;
  u32 alloc_allocated; // capacity
  u32 size;
  u32 allocated;
  u32 freed;
  char file[256];
}ST_MemAllocLine;

// global tracking array
ST_MemAllocLine shiba_alloc_lines[1024];
// track how many lines are in use
u32 shiba_alloc_line_count = 0;

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
  u32 k;
  
  if (shiba_alloc_mutex)
    shiba_alloc_mutex_lock(shiba_alloc_mutex);

  for (int i = 0; i < shiba_alloc_line_count; i++) {
    for (int j = 0; j < shiba_alloc_lines[i].alloc_count; j++)
    {
      u8* buf = shiba_alloc_lines[i].allocs[j].buffer;
      u32 size = shiba_alloc_lines[i].allocs[j].size;

      // check if any of the extra allocated bytes are NOT
      // our magic number. This means we wrote too far if so.
      for (k = 0; k < SHIBA_MEM_OVER_ALLOC; k++) {
        if (buf[size + k] != SHIBA_MEM_MAGIC_NUM)
          break;
      }

      if (k < SHIBA_MEM_OVER_ALLOC) {
        fprintf(stderr, "MEMORY ERROR: We wrote past the end of our allocated buffer at line %u in file %s!\n", shiba_alloc_lines[i].line, shiba_alloc_lines[i].file);

        // kill it
        {
          u32* pay_attention_bro = NULL;
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

void shiba_memory_debug_add(void* ptr, u32 size, char* file, u32 line) {
  u32 i, j;

  for (i = 0; i < SHIBA_MEM_OVER_ALLOC; i++)
    ((u8*) ptr)[size + i] = SHIBA_MEM_MAGIC_NUM;

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

      for (j = 0; j < 255 && file[j] != 0; j++) {
        shiba_alloc_lines[i].file[j] = file[j];
      }

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

void* shiba_memory_debug_malloc(u32 size, char* file, u32 line) {
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

  // initialize the entire allocation with 133s
  // (canary bytes are 132), 133 means they havent written
  // here yet
  for (u32 i = 0; i < size + SHIBA_MEM_OVER_ALLOC; i++) {
    ((u8* ) ptr)[i] = SHIBA_MEM_MAGIC_NUM + 1;
  }

  shiba_memory_debug_add(ptr, size, file, line);

  if (shiba_alloc_mutex)
    shiba_alloc_mutex_unlock(shiba_alloc_mutex);

  return ptr;
}

boolean shiba_memory_debug_remove(void* buf) {
  int k;
  for (int i = 0; i < shiba_alloc_line_count; i++) {
    for (int j = 0; j < shiba_alloc_lines[i].alloc_count; j++)
    {
      if (shiba_alloc_lines[i].allocs[j].buffer == buf)
      {
        for (k = 0; k < SHIBA_MEM_OVER_ALLOC; k++) {
          if (((u8* ) buf)[shiba_alloc_lines[i].allocs[j].size + k] != SHIBA_MEM_MAGIC_NUM)
            break;
        }

        if (k < SHIBA_MEM_OVER_ALLOC)
          fprintf(stderr, "MEMORY ERROR: Wrote past allocated buffer at line %u in file %s!\n", shiba_alloc_lines[i].line, shiba_alloc_lines[i].file);

        shiba_alloc_lines[i].size -= shiba_alloc_lines[i].allocs[j].size;
        shiba_alloc_lines[i].allocs[j] = shiba_alloc_lines[i].allocs[--shiba_alloc_lines[i].alloc_count];
        shiba_alloc_lines[i].freed++;

        return TRUE;
      }
    }
  }
  return FALSE;
}

void* shiba_memory_debug_realloc(void* ptr, u32 size, char* file, u32 line) {
  void* ptr2; u32 i, j, k, move;

  if (!ptr)
    return shiba_memory_debug_malloc(size, file, line);

  if (shiba_alloc_mutex)
    shiba_alloc_mutex_lock(shiba_alloc_mutex);

  for (i = 0; i < shiba_alloc_line_count; i++)
  {
    for (j = 0; j < shiba_alloc_lines[i].alloc_count; j++)
      if (shiba_alloc_lines[i].allocs[j].buffer == ptr)
        break;

    if (j < shiba_alloc_lines[i].alloc_count)
      break;
  }
    
    if (i == shiba_alloc_line_count) {
      fprintf(stderr, "MEMORY ERROR: Rellocation of pointer %p in %s line %u failed. Pointer was never allocated!\n", ptr, file, line);

      for (i = 0; i < shiba_alloc_line_count; i++)
      {
        u32* buf = shiba_alloc_lines[i].allocs[j].buffer;
        for (k = 0; k < shiba_alloc_lines[i].allocs[j].size; k++) {
          if (&buf[k] == ptr)
            printf("Trying to reallocate pointer of %u bytes (out of %u) into alloc made in %s on line %u!\n", k, shiba_alloc_lines[i].allocs[j].size, shiba_alloc_lines[i].file, shiba_alloc_lines[i].line);
      }
    }
    exit(0);
  }

  move = shiba_alloc_lines[i].allocs[j].size;

  if (size < move)
    move = size;

  ptr2 = malloc(size + SHIBA_MEM_OVER_ALLOC);

  if (!ptr2) {
    fprintf(stderr, "MEMORY ERROR: Realloc returns NULL trying to alloc %u bytes at line %u in file %s!\n", size, line, file);

    if (shiba_alloc_mutex)
      shiba_alloc_mutex_unlock(shiba_alloc_mutex);

    shiba_memory_debug_print(0);
    exit(0);
  }

  for (i = 0; i < size + SHIBA_MEM_OVER_ALLOC; i++)
      ((u8* ) ptr2)[i] = SHIBA_MEM_MAGIC_NUM + 1;

  memcpy(ptr2, ptr, move);

  shiba_memory_debug_add(ptr2, size, file, line);
  shiba_memory_debug_remove(ptr);
  free(ptr);

  if (shiba_alloc_mutex)
  shiba_alloc_mutex_unlock(shiba_alloc_mutex);

return ptr2;
}

void shiba_memory_debug_free(void* buffer) {
  if (shiba_alloc_mutex)
    shiba_alloc_mutex_lock(shiba_alloc_mutex);

  if (!shiba_memory_debug_remove(buffer)) {
    u32* pay_attention_bro = NULL;
    pay_attention_bro[0] = 420;
  }
  free(buffer);

  if (shiba_alloc_mutex)
    shiba_alloc_mutex_unlock(shiba_alloc_mutex);
}

void shiba_memory_debug_print(u32 min_allocations) {
  if (shiba_alloc_mutex)
    shiba_alloc_mutex_lock(shiba_alloc_mutex);

  printf("\n<--------------------- MEMORY STATUS ---------------------->\n");

  for (int i = 0; i < shiba_alloc_line_count; i++) {
    if (min_allocations < shiba_alloc_lines[i].allocated) {
      printf("File %s at line: %u!\n", shiba_alloc_lines[i].file, shiba_alloc_lines[i].line);
      printf(" - Bytes allocated: %u\n - Allocations: %u\n - Frees: %u\n\n", shiba_alloc_lines[i].size, shiba_alloc_lines[i].allocated, shiba_alloc_lines[i].freed);
    }
  }
  printf("<---------------------------------------------------------->\n");

  if (shiba_alloc_mutex)
    shiba_alloc_mutex_unlock(shiba_alloc_mutex);
}

void shiba_memory_debug_reset() {
  if (shiba_alloc_mutex)
    shiba_alloc_mutex_lock(shiba_alloc_mutex);

  for (int i = 0; i < shiba_alloc_line_count; i++)
    free(shiba_alloc_lines[i].allocs);

  shiba_alloc_line_count = 0;

  if (shiba_alloc_mutex)
    shiba_alloc_mutex_unlock(shiba_alloc_mutex);
}

u32 shiba_memory_debug_mem_usage() {
  u32 sum = 0;
  if (shiba_alloc_mutex)
    shiba_alloc_mutex_lock(shiba_alloc_mutex);

  for (int i = 0; i < shiba_alloc_line_count; i++) {
    sum += shiba_alloc_lines[i].size;
  }

  if (shiba_alloc_mutex)
    shiba_alloc_mutex_unlock(shiba_alloc_mutex);

  return sum;
}
