#include "./scan-engine.h"

int open_tcp_connect(scan_info_t* s, const u16 port) {
  int ret_val = 1;

  if (shiba_network_return_ip_type(s->targets->target) == 4)
    s->af = AF_INET;
  else
    s->af = AF_INET6;

  shiba_network_socket_t* socket = shiba_network_create_socket(s->af, s->sock_type, 0);
  if (!socket)
    return ret_val;

  struct sockaddr_storage addr = {0};

  if (s->af == AF_INET) {
    struct sockaddr_in* new_addr = (struct sockaddr_in*)&addr;
    new_addr->sin_family = s->af;
    new_addr->sin_port = htons(port);

    int pton_res = inet_pton(s->af, s->targets->target, &new_addr->sin_addr.s_addr);
    if (pton_res <= 0) {
      goto Cleanup;
    }

    #ifdef _WIN32
      // windows stuff
    #else
      struct timeval timeout;
      timeout.tv_sec = s->timeout;
      setsockopt(socket->handle, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
      setsockopt(socket->handle, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    #endif

    int res = connect(socket->handle, (struct sockaddr*)&addr, sizeof(addr));
    if (res == 0) {
      ret_val = 0;
    }
  }
  else {
    // TODO: IPv6 work
    ret_val = 0;
  }

  Cleanup:
    shiba_network_destroy_socket(socket);
    return ret_val;
}
