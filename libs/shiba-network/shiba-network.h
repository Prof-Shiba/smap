#pragma once

#include "../shiba-core/shiba.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#if defined _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

typedef struct shiba_network_socket_t {
  #if defined _WIN32
      SOCKET handle;
  #else
      int handle;
  #endif
}shiba_network_socket_t;

/* validates an ip, whether v4 or v6. returns 4 on IPv4 success,
 * 6 on IPv6 success, or 1 if invalid */
extern int shiba_network_is_valid_ip_address(const char* ip_addr);
extern boolean shiba_network_is_valid_port(const int32 port);

/* Init and cleanup functions needed for windows */
extern boolean shiba_network_init(void);
extern void shiba_network_cleanup(void);

/*Creates a shiba_network_socket_t* and returns it. Returns NULL if fails */
extern shiba_network_socket_t* shiba_network_create_socket(int AF, int TYPE, int PROTOCOL);
/* Closes and frees a shiba_network_socket_t* */
extern void shiba_network_destroy_socket(shiba_network_socket_t* socket);
