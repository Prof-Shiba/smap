#include "./scan-engine.h"

// TODO: 1. -st TCP-Connect Scanning
// 2. -ss SYN scanning
// 2. -sf FIN scanning
// 2. -su UDP scanning
// 2. -sn ping scanning

int is_valid_ip(const char* target) {
#ifdef _WIN32
#else
  char buf[INET6_ADDRSTRLEN];

  if (inet_pton(AF_INET, target, buf))
    return 4;
  else if (inet_pton(AF_INET6, target, buf))
    return 6;

#endif
  return 1;
}
