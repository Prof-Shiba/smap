#pragma once
// Shoutout to nmap for this :)

#include "../shiba-core/shiba.h"
#include <string.h>

#define no_argument 0
#define required_argument 1
#define optional_argument 2

struct option {
  const char* name;
  int* flag;
  int has_arg;
  int val;
};

extern int opt_index, opt_err, opt_opt;
extern char* opt_arg;

extern int getopt_reset(void);

extern int getopt_long(int argc, char* argv[], const char* short_opts,
    const struct option* long_opts, int* long_index);

extern int getopt_long_only(int argc, char* argv[], const char* short_opts,
    const struct option* long_opts, int* long_index);

extern int _getopt_internal(int argc, char* argv[], const char* short_opts,
    const struct option* long_opts, int* long_index, int long_only);
