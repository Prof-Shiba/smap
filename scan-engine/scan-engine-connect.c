#include "./scan-engine.h"

// NOTE: When scanning loopback addresses, theres a small chance the src and
// dst port will be the same, resulting in a false positive. This must be navigated around on linux.
// this race condition only occurs on localhost because the source and destination IPs are identical,
// (src IP, src port, dst IP, dst port) collapse into a loop if src port == dst port
// https://stackoverflow.com/questions/24077625/how-to-see-if-i-connected-to-an-ephemeral-port - 186 views from 11 yrs ago
//
// I spent countless hours debugging this 100% convinced it was my fault until i began to give up
// and suspect it was a kernel issue...and it was. Im going to leave this comment here because it was
// absurdly annoying and i unironically started to lose my mind. I'll probably change this open_tcp_connect
// function in the future anyways as im sure theres a better way to do things but it works for now.

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

    // TODO: Check if its a loopback addr before doing all this. Also validate for windows!
    struct sockaddr_in src_addr = {0};
    src_addr.sin_family = AF_INET;
    src_addr.sin_addr.s_addr = INADDR_ANY;
    src_addr.sin_port = 0; // force kernel to pick the port early (0 means pick any random ephemeral port)
    
    // TODO: Add better error checking/handling on these. We dont wanna miss sockets.
    if (bind(socket->handle, (struct sockaddr*)&src_addr, sizeof(src_addr)) < 0) {
        goto Cleanup;
    }

    struct sockaddr_in assigned;
    socklen_t len = sizeof(assigned);
    // give us the port the stupid ass kernel chose
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

    #ifdef _WIN32
      // windows stuff
    #else
      struct timeval timeout;
      timeout.tv_sec = 0;
      timeout.tv_usec = s->timeout;
      setsockopt(socket->handle, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
      setsockopt(socket->handle, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    #endif

    int res = connect(socket->handle, (struct sockaddr*)&addr, sizeof(addr));
    if (res == 0) {
      ret_val = 0;
    }
  }
  else {
    // NOTE: Untested. Interrupted by bug with unknown characters spamming screen.
    struct sockaddr_in6* new_addr6 = (struct sockaddr_in6*)&addr;
    new_addr6->sin6_family = AF_INET6;
    new_addr6->sin6_port = htons(port);

    struct sockaddr_in6 src_addr = {0};
    src_addr.sin6_family = AF_INET6;
    src_addr.sin6_addr = in6addr_any;
    src_addr.sin6_port = 0;
    
    // TODO: Add better error checking/handling on these. We dont wanna miss sockets.
    if (bind(socket->handle, (struct sockaddr*)&src_addr, sizeof(src_addr)) < 0) {
        goto Cleanup;
    }

    struct sockaddr_in6 assigned;
    socklen_t len = sizeof(assigned);
    if (getsockname(socket->handle, (struct sockaddr*)&assigned, &len) < 0) {
        goto Cleanup;
    }

    u16 src_port = ntohs(assigned.sin6_port);
    if (src_port == port) {
      goto Retry;
    }

    int pton_res = inet_pton(s->af, s->targets->target, &new_addr6->sin6_addr.s6_addr);
    if (pton_res <= 0) {
      goto Cleanup;
    }

    ret_val = 0;
  }

  Cleanup:
    shiba_network_destroy_socket(socket);
    return ret_val;

  Retry:
    shiba_network_destroy_socket(socket);
    return open_tcp_connect(s, port); // tail recursion should be optimized away by compiler
}
