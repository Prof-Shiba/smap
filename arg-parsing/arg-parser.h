#pragma once

// This contains argument parsing logic as well as the
// print_usage and version functions, which are only used here

#include "../smap-options/smap-options.h"

int parse_args(int argc, char* argv[], scan_info_t* s);
void print_usage(char* argv[]);
void print_version(char* argv[]);
void link_ips(int argc, char* argv[], scan_info_t* s);
