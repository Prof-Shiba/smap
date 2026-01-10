#pragma once

// This file includes things such as scan information, port state information,
// scan types, and functions needed to help with these

#include "../libs/shiba-core/shiba.h"
#include "../libs/shiba-network/shiba-network.h"
#include <string.h>

#define MAX_PORT 65535

#ifndef _WIN32
  #include <unistd.h>
  #include <sys/socket.h>
#else
  #define WIN32_LEAN_AND_MEAN
  #include <winsock2.h>
  #include <ws2tcpip.h>
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
  PORT_OPEN,
  PORT_CLOSED,
  PORT_FILTERED,
  PORT_FAILED,
  PORT_UNKNOWN
} port_state_t;

typedef struct port_t {
  port_state_t state;
  u16 port_num;
} port_t;

typedef struct target_t {
  struct target_t* next;
  char* target;
} target_t;

typedef struct {
  port_t port_list[MAX_PORT + 1]; // we arent using element 0
  scan_type_t scan_type;
  target_t* targets;
  u32* port_nums;
  u32 timeout;
  u16 port_capacity;
  u16 num_ports_to_scan;
  u16 closed_ports;
  u16 open_ports;
  u16 ignored_ports;
  u16 af;
  u16 sock_type;
} scan_info_t;

extern boolean check_if_root(void);
extern void init_scan_info(scan_info_t* s);
extern void scan_info_cleanup(scan_info_t* s);
