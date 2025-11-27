#pragma once
#include "../libs/shiba-core/shiba.h"
#include "../libs/shiba-network/shiba-network.h"
#include "../smap-options/smap-options.h"
#include "./scan-engine-connect.h"

// TODO: How will we scan multiple different IPs at once and store all that?
// scan_info_t may have to change, and *possibly* scan_ports?

// These functions will handle collecting responses and doing the general
// networking business needed to collect information about the ports

// scan all hosts and their ports marked by s->port_list
extern void scan_ports(scan_info_t* s);

// scan an individual port by calling the appropriate function
// for the scan type specified
extern int scan_port(scan_info_t* s, const uint16 port);

// set the ports that need to be ignored
extern void set_ignored_ports(scan_info_t* s);
