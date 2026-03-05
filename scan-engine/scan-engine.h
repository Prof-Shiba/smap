#pragma once

/*
  This header file declares functions that will eventually call
  the actual port scanning functions, such as open_tcp_connect.
  As well as other minor things that don't fit elsewhere, such as
  setting the number of ports we are ignoring on a target.
 */

#include "../libs/shiba-core/shiba.h"
#include "../libs/shiba-network/shiba-network.h"
#include "../smap-options/smap-options.h"
#include "./scan-engine-connect.h"

// scan all hosts and their ports marked by s->port_list
extern void scan_ports(scan_info_t* s);

// scan an individual port by calling the appropriate function
// for the scan type specified
extern int scan_port(scan_info_t* s, const u16 port);

// set the ports that need to be ignored
extern void set_ignored_ports(scan_info_t* s);
