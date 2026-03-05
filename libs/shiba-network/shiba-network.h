#pragma once

#include "../shiba-core/shiba.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#if defined _WIN32
  #include <WinSock2.h>
  #include <ws2tcpip.h>
#else
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <sys/time.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <netdb.h>
  #include <netinet/ip_icmp.h>
  #include <netinet/icmp6.h>
#endif

typedef struct shiba_network_socket_t {
  #if defined _WIN32
      SOCKET handle;
  #else
      int handle;
  #endif
}shiba_network_socket_t;

/* Returns an IP type, whether IPv4 or IPv6. 
 * returns 4 on IPv4 success, 6 on IPv6 success, or 1 if invalid
 * This can also be used generally to validate if a given IP is valid */
extern int shiba_network_return_ip_type(const char* ip_addr);
extern boolean shiba_network_is_valid_port(const i32 port);

/* Init and cleanup functions needed for windows */
extern boolean shiba_network_init(void);
extern void shiba_network_cleanup(void);

/*Creates a shiba_network_socket_t pointer and returns it. Returns NULL if fails. 
 * NOTE: Call shiba_network_destroy_socket after to free! */
extern shiba_network_socket_t* shiba_network_create_socket(const int af, const int type, const int protocol);
/* Closes and frees a shiba_network_socket_t* */
extern void shiba_network_destroy_socket(shiba_network_socket_t* socket);
