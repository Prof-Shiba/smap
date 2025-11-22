#include "../libs/shiba-network/shiba-network.h"
#include "../smap-options/smap-options.h"

// This file will contain functions for the TCP connect
// portion of scanning, as we don't need raw sockets for it

// Open a normal TCP connection. Returns 0 on success, 1 on failure.
extern int open_tcp_connect(scan_info_t* s, const uint16 port);
