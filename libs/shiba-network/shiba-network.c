#include "shiba-network.h"

static WSADATA wsa_data;

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

// This currently is basically only for windows,
// but in the future, maybe some UNIX stuff can
// get done here.
boolean shiba_network_init(void) {
#if defined _WIN32
  int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
  if (result != 0) return FALSE;

  return TRUE;
#else
  // we can do linux stuff here
#endif
}

void shiba_network_cleanup(void) {
#if defined _WIN32
  WSACleanup();
#else
  // linux stuff here
#endif
}
