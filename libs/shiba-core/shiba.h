#pragma once
/*  _____                       */
/* |_   _|   _ _ __   ___  ___  */
/*   | || | | | '_ \ / _ \/ __| */
/*   | || |_| | |_) |  __/\__ \ */
/*   |_| \__, | .__/ \___||___/ */
/*       |___/|_|               */

#if !defined(TYPES_H)
#define TYPES_H

#if defined _WIN32
typedef unsigned int uint;
#else
#include <sys/types.h>
#endif

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef float fint32;
typedef double fint64;
typedef unsigned char boolean;
typedef signed long long int64;
typedef unsigned long long uint64;

#define PI 3.14159265358979323846

#if !defined(SHIBA_MEM_DEBUG_OFF)
#define SHIBA_MEM_DEBUG
#endif

#ifdef SHIBA_MEM_DEBUG

/*  ____       _                       _              */
/* |  _ \  ___| |__  _   _  __ _  __ _(_)_ __   __ _  */
/* | | | |/ _ \ '_ \| | | |/ _` |/ _` | | '_ \ / _` | */
/* | |_| |  __/ |_) | |_| | (_| | (_| | | | | | (_| | */
/* |____/ \___|_.__/ \__,_|\__, |\__, |_|_| |_|\__, | */
/*                         |___/ |___/         |___/  */

// This is used for things such as tracking memory allocations
// made with malloc to check for leaks. As well as checking if 
// we wrote outside of program memory.

extern void shiba_memory_debug_init(void (*lock)(void* mutex), void (*unlock)(void* mutex), void* mutex);
extern void shiba_memory_debug_malloc(uint size, char* file, uint line);
extern void shiba_memory_debug_realloc(void* ptr, uint size, char* file, uint line);
extern void shiba_memory_debug_free(void* buffer);
extern void shiba_memory_debug_print(uint min_allocations);
extern void shiba_memory_debug_reset();
extern boolean shiba_memory_debug();

#define malloc(n) shiba_memory_debug_malloc(n, __FILE__, __LINE__)
#define realloc(n, m) shiba_memory_debug_realloc(n, m, __FILE__, __LINE__)
#define free(n) shiba_memory_debug_free(n)
#endif


/*  _____      _              */
/* | ____|_  _| |_ _ __ __ _  */
/* |  _| \ \/ / __| '__/ _` | */
/* | |___ >  <| |_| | | (_| | */
/* |_____/_/\_\\__|_|  \__,_| */

extern uint shiba_random_int(uint32 num); // pseudo-random num gen

#endif
