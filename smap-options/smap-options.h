#pragma once
#include "../libs/shiba-core/shiba.h"

#define WIN32_LEAN_AND_MEAN 

#ifndef _WIN32
#include <unistd.h>
#else
#include <Windows.h>
#endif

typedef enum {
  SCAN_TCP,
  SCAN_UDP,
  SCAN_SYN,
  SCAN_FIN,
  SCAN_PING
} scan_type_t;

typedef enum {
  OPEN,
  CLOSED,
  FILTERED,
  FAILED
} port_state_t;

extern boolean check_if_root(void);
