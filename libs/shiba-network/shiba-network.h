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

extern boolean shiba_network_is_valid_ipv4_address(const char* ip_addr);
extern boolean shiba_network_is_valid_port(const int32 port);

// will need a create_socket function
// and 
