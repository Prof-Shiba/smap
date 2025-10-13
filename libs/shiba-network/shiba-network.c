#include "shiba-network.h"


boolean shiba_network_is_valid_ipv4_address(const char* ip_addr) {
  if (!ip_addr)
    return 0;

  #ifdef _WIN32
  int result = inet_addr(ip_addr);
  if (result == INADDR_NONE || result == INADDR_ANY)
      return 0;
  return 1;
  #else
  struct in_addr addr;
  return inet_pton(AF_INET, ip_addr, &addr) == 1;
  #endif
}

boolean shiba_network_is_valid_port(const int32 port) {
  if (!port)
    return 0;

  if (port < 1 || port > 65535)
    return 0;

  return 1;
}
