#pragma once
#include "../shiba-core/shiba.h"

// TODO: Create a cross-platform 'getopt_long' function to use for
// general purpose arg parsing
struct option {
  const char* name;
  int has_arg;
  int* flag;
  int val;
};

#define no_argument 0
#define required_argument 1
#define optional_argument 2
