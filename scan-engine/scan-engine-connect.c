#include "./scan-engine.h"
 
// NOTE: When scanning loopback addresses on linux, theres a small chance the src and
// dst port will be the same, resulting in a false positive. This must be navigated around.
// this condition only occurs on localhost because the source and destination IPs can be identical,
// (src IP, src port, dst IP, dst port) collapse into a loop if src port == dst port
// https://stackoverflow.com/questions/24077625/how-to-see-if-i-connected-to-an-ephemeral-port - 186 views from 11 yrs ago
//
// I spent countless hours debugging this 100% convinced it was my fault until i began to give up
// and suspect it was a kernel issue...and it was. Im going to leave this comment here because it was
// absurdly annoying and i unironically started to lose my mind. TCP isn't as good as say SYN scanning anyways
// typically, but it's still worth being aware that this can happen

int open_tcp_connect(scan_info_t* s, const u16 port) {
  static const int max_num_retries = 5;
  static int num_retries = 0;
  static int ret_val = 1;

  while (num_retries < max_num_retries) {
    ret_val = 1;

    if (shiba_network_return_ip_type(s->targets->target) == 4)
      s->af = AF_INET;
    else
      s->af = AF_INET6;

    shiba_network_socket_t* socket = shiba_network_create_socket(s->af, s->sock_type, 0);
    if (!socket)
      return ret_val;

    // set non-blocking stuff here
    #ifdef _WIN32
      // If opt_val == 0, blocking is enabled; Sockets block by default
      // If opt_val != 0, non-blocking mode is enabled.
      u_long opt_val = 1;
      int ret = ioctlsocket(socket->handle, FIONBIO, &opt_val);
      if (ret == SOCKET_ERROR) {
        goto Retry;
      }
    #else
      int flags = fcntl(socket->handle, F_GETFL, 0);
      if (flags < 0) {
        goto Retry;
      }

      flags |= O_NONBLOCK;
      if (fcntl(socket->handle, F_SETFL, flags) < 0) {
        goto Retry;
      }
    #endif

    struct sockaddr_storage addr = {0};

    if (s->af == AF_INET) {
      struct sockaddr_in* new_addr = (struct sockaddr_in*)&addr;
      new_addr->sin_family = s->af;
      new_addr->sin_port = htons(port);

      // TODO: Check if its a loopback addr before doing all this.
      struct sockaddr_in src_addr = {0};
      src_addr.sin_family = AF_INET;
      src_addr.sin_addr.s_addr = INADDR_ANY;
      src_addr.sin_port = 0; // force kernel to pick the port early (0 means pick any random ephemeral port)
      
      // TODO: Add better error checking/handling on these. We dont wanna miss sockets.
      if (bind(socket->handle, (struct sockaddr*)&src_addr, sizeof(src_addr)) < 0) {
          goto Cleanup;
      }

      struct sockaddr_in assigned = { 0 };
      socklen_t len = sizeof(assigned);
      if (getsockname(socket->handle, (struct sockaddr*)&assigned, &len) < 0) {
          goto Cleanup;
      }

      u16 src_port = ntohs(assigned.sin_port);
      if (src_port == port) {
        goto Retry;
      }

      int pton_res = inet_pton(s->af, s->targets->target, &new_addr->sin_addr.s_addr);
      if (pton_res <= 0) {
        goto Cleanup;
      }

      int res = connect(socket->handle, (struct sockaddr*)&addr, sizeof(addr));
      if (res == 0) {
        ret_val = 0; // might get hit on localhost scans
      }
      else {
        #ifdef _WIN32
          // TODO: check if host is unreachable
          if (res == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
            goto Cleanup;
          }
        #else
          // TODO: check if host is unreachable
          if (errno != EINPROGRESS) {
            goto Cleanup;
          }
        #endif
      }

        fd_set write_fd, except_fd;
        FD_ZERO(&write_fd);
        FD_SET(socket->handle, &write_fd);
        FD_ZERO(&except_fd);
        FD_SET(socket->handle, &except_fd);

        struct timeval tv = {
          .tv_sec  = s->timeout / 1000,
          .tv_usec = (s->timeout % 1000) * 1000
        };

        res = select(socket->handle + 1, NULL, &write_fd, &except_fd, &tv);
        if (res < 0) { // hitting this infinitely on windows and overflowing stack when res <= 0. res equaled 0.
          goto Retry;
        }
        else if (res == 0) {
          goto Cleanup;
        }
        if (FD_ISSET(socket->handle, &except_fd)) {
          goto Cleanup;
        }

        int error = 0;
        socklen_t error_len = sizeof(error);
        if (getsockopt(socket->handle, SOL_SOCKET, SO_ERROR, &error, &error_len) == -1 || error != 0) {
          goto Cleanup;
        }

        ret_val = 0;
    }
    else {
      struct sockaddr_in6* new_addr6 = (struct sockaddr_in6*)&addr;
      new_addr6->sin6_family = s->af;
      new_addr6->sin6_port = htons(port);

      // TODO: Check if its a loopback addr before doing all this.
      struct sockaddr_in6 src_addr = {0};
      src_addr.sin6_family = AF_INET6;
      src_addr.sin6_addr = in6addr_any;
      src_addr.sin6_port = 0;
      
      // TODO: Add better error checking/handling on these. We dont wanna miss sockets.
      if (bind(socket->handle, (struct sockaddr*)&src_addr, sizeof(src_addr)) < 0) {
          goto Cleanup;
      }

      struct sockaddr_in assigned = { 0 };
      socklen_t len = sizeof(assigned);
      if (getsockname(socket->handle, (struct sockaddr*)&assigned, &len) < 0) {
          goto Cleanup;
      }

      u16 src_port = ntohs(assigned.sin_port);
      if (src_port == port) {
        goto Retry;
      }

      int pton_res = inet_pton(s->af, s->targets->target, &new_addr6->sin6_addr.s6_addr);
      if (pton_res <= 0) {
        goto Cleanup;
      }

      int res = connect(socket->handle, (struct sockaddr*)&addr, sizeof(addr));
      if (res == 0) {
        ret_val = 0; // might get hit on localhost scans
      }
      else {
        #ifdef _WIN32
          if (res == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
            goto Cleanup;
          }
        #else
          if (errno != EINPROGRESS) {
            goto Cleanup;
          }
        #endif
      }

        fd_set write_fd, except_fd;
        FD_ZERO(&write_fd);
        FD_SET(socket->handle, &write_fd);
        FD_ZERO(&except_fd);
        FD_SET(socket->handle, &except_fd);

        struct timeval tv = {
          .tv_sec  = s->timeout / 1000,
          .tv_usec = (s->timeout % 1000) * 1000
        };

        res = select(socket->handle + 1, NULL, &write_fd, &except_fd, &tv);
        if (res < 0) { // hitting this infinitely on windows and overflowing stack when res <= 0. res equaled 0.
          goto Retry;
        }
        else if (res == 0) {
          goto Cleanup;
        }
        if (FD_ISSET(socket->handle, &except_fd)) {
          goto Cleanup;
        }

        int error = 0;
        socklen_t error_len = sizeof(error);
        if (getsockopt(socket->handle, SOL_SOCKET, SO_ERROR, &error, &error_len) == -1 || error != 0) {
          goto Cleanup;
        }

      ret_val = 0;
    }

    Cleanup:
      shiba_network_destroy_socket(socket);
      return ret_val;

    Retry:
      num_retries++;
      shiba_network_destroy_socket(socket);
      continue;
    }
  return ret_val;
}
