#include "./shiba-getopt.h"

int opt_index = 1, opt_err = 1, opt_opt = 1;
char* opt_arg = 0;

int getopt_reset(void) {
  opt_index = 1;
  opt_err = 1;
  opt_opt = 0;
  opt_arg = 0;
  return 0;
}

int _getopt_internal(int argc, char* argv[], const char* short_opts,
            const struct option* long_opts, int* long_index, int long_only)
{
  // TODO:
}

int getopt_long(int argc, char* argv[], const char* short_opts,
            const struct option* long_opts, int* long_index)
{
  return _getopt_internal(argc, argv, short_opts, long_opts, long_index, 0);
}

int getopt_long_only(int argc, char* argv[], const char* short_opts,
            const struct option* long_opts, int* long_index)
{
  return _getopt_internal(argc, argv, short_opts, long_opts, long_index, 1);
}
