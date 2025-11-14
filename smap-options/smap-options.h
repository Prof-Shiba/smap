#pragma once

// This file includes things such as scan information, port state information,
// scan types, and functions needed to help with these

#include "../libs/shiba-core/shiba.h"
#define MAX_PORT 65535

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 
#endif

#ifndef _WIN32
#include <unistd.h>
#else
#include <Windows.h>
#endif

typedef struct target_t {
  char* target;
  struct target_t* next;
} target_t;

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
  target_t* targets;
  boolean port_list[MAX_PORT + 1];
  scan_type_t scan_type;
  uint16 num_ports;
  uint16 closed_ports;
  uint16 open_ports;
  uint16 ignored_ports;
} scan_info_t;

extern boolean check_if_root(void);
extern void init_scan_info(scan_info_t* s);
extern void scan_info_cleanup(scan_info_t* s);
