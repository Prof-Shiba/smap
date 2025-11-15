#include "shiba-network.h"

#if defined _WIN32
  static WSADATA wsa_data;
#endif

int shiba_network_is_valid_ip_address(const char* ip_addr) {
  if (!ip_addr)
    return 1;

#ifdef _WIN32
  // FIXME: add ipv6 checks too
  int result = inet_addr(ip_addr);
  if (result == INADDR_NONE || result == INADDR_ANY)
      return 1;
  return 0;
#else
  char buf[INET6_ADDRSTRLEN];

  if (inet_pton(AF_INET, ip_addr, buf))
    return 4;
  else if (inet_pton(AF_INET6, ip_addr, buf))
    return 6;
#endif

return 1;
}

boolean shiba_network_is_valid_port(const int32 port) {
  if (!port)
    return 1;

  if (port < 1 || port > 65535)
    return 1;

  return 0;
}

// This currently is basically only for windows,
// but in the future, maybe some UNIX stuff can
// get done here.
boolean shiba_network_init(void) {
#if defined _WIN32
  int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
  if (result != 0) {
    fprintf(stderr, "WSA Startup Failed! Error: %d\n", GetLastError());
    exit(1);
  }

  return TRUE;
#else
  return TRUE; // not doing anything on linux rn
#endif
}

void shiba_network_cleanup(void) {
#if defined _WIN32
  WSACleanup();
#else
  // linux stuff here
#endif
}

// NOTE: If we need sudo privs, then kill it with a message. otherwise
// it is probably recoverable. we may run out of FD's when doing multi-threading.
// It should be left up to the caller how to handle the error.
shiba_network_socket_t* shiba_network_create_socket(int AF, int TYPE, int PROTOCOL) {
shiba_network_socket_t* sock = malloc(sizeof(*sock));
if (!sock) return NULL;
#if defined _WIN32
  SOCKET s = socket(AF, TYPE, PROTOCOL);

  // NOTE: check here if smth fails:
  // https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket
  if (s == INVALID_SOCKET) {
    WSACleanup();
    return NULL;
  }

  sock->handle = s;
  return sock;
#else
  int socket_fd = socket(AF, TYPE, PROTOCOL);
  if (socket_fd == -1) {
    return NULL;
  }

  sock->handle = socket_fd;
  return sock;
#endif
}

void shiba_network_destroy_socket(shiba_network_socket_t* socket) {
if (!socket) return;
#if defined _WIN32
  int close_result = closesocket(socket->handle);
  if (close_result == SOCKET_ERROR) {
    WSACleanup();
    free(socket);
    exit(1);
  }
#else
  close(socket->handle);
#endif
  free(socket);
}
