#pragma once
#include "../libs/shiba-core/shiba.h"
#include "../smap-options/smap-options.h"
#include <string.h>

// This file contains port handling functions, like parsing ports
// such as -p 21,22,445,1337 or -p- for all 65535 ports

// TODO: Port ranges: -p 1-1024

// ports must be passed in without spaces, separated by commas. Returns 1 on fail, 0 on success
extern int get_ports(char* port_list, scan_info_t* s);
