#include "./smap-options.h"

boolean check_if_root(void) {
#ifndef _WIN32
  if (geteuid() == 0)
    return TRUE;
  return FALSE;
#else
  // TODO: windows
#endif
}
