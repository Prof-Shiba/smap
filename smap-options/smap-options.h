#pragma once

// This file includes things such as scan information, port state information,
// scan types, and functions needed to help with these

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

typedef struct {
  char* address;
  uint16* port_list;
  scan_type_t scan_type; // enums are 4 bytes
  uint16 num_ports;
  uint16 closed_ports;
  uint16 open_ports;
} scan_info_t;

extern boolean check_if_root(void);
