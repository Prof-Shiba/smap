#pragma once
#include "../libs/shiba-core/shiba.h"
#include "../smap-options/smap-options.h"
#include <string.h>

// TODO: Port ranges: -p 1-1024

// This file contains port handling functions, like parsing ports
// such as -p 21,22,445,1337 or -p- for all 65535 ports

// parses and gets ports entered by user. Kills program on fail, 0 on success
extern int get_ports(const char* ports, scan_info_t* s);
extern void insertion_sort(u32 arr[], int length);
