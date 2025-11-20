#pragma once
#include "../libs/shiba-core/shiba.h"
#include "../libs/shiba-network/shiba-network.h"
#include "../smap-options/smap-options.h"

// These functions will handle collecting responses and doing the general
// networking business needed to collect information about the ports

// scan all ports marked by s->port_list
void scan_ports(const scan_info_t* s);

// scan an individual port by calling the appropriate function
// for the scan type specified
int scan_port(const scan_info_t* s, const uint16 port);
