#pragma once

#include "../shiba-core/shiba.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#if defined _WIN32
// windows stuff here
// ill add this once i know
// how to work with windows sockets
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

extern boolean is_valid_ipv4_address(const char* ip_addr);
extern boolean is_valid_port(const uint16 port);
