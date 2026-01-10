#pragma once

// This contains argument parsing logic as well as the
// print_usage and version functions, which are only used here

#include "../smap-options/smap-options.h"

extern int parse_args(int argc, char* argv[], scan_info_t* s);
extern void parse_timeout(const char* opt_arg, scan_info_t* s);
extern void print_usage(char* argv[]);
extern void print_version(char* argv[]);
extern void link_ips(int argc, char* argv[], scan_info_t* s);
