#include "./scan-engine.h"

int open_tcp_connect(scan_info_t* s, const uint16 port) {
  shiba_network_socket_t* socket = shiba_network_create_socket(s->af, s->sock_type, 0);
  if (!socket) return 1;
  struct sockaddr_storage addr;
  socklen_t addr_len;

  if (shiba_network_is_valid_ip_address(s->targets->target) == 4)
    s->af = AF_INET;
  else
    s->af = AF_INET6;

  if (s->af == AF_INET) {
    struct sockaddr_in* new_addr = (struct sockaddr_in*)&addr;
    new_addr->sin_family = s->af;
    new_addr->sin_port = port;
    // TODO: Make this function
    shiba_network_inet_pton(s->af, s->targets->target, &new_addr->sin_addr.s_addr);
  }
  else {
    // TODO: IPv6 work
  }


  shiba_network_destroy_socket(socket);
  return 0;
}
