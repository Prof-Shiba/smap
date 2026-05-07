#include "./host-alive.h"

u16 icmp_checksum(u16* data, size_t size) {
  u32 sum = 0;
  size_t data_len = size / 2;

  for (size_t i = 0; i < data_len; i++) {
    sum += data[i];
  }

  // fold 32 bits to 16
  return ~((sum << 16 >> 16) + (sum >> 16));
}

boolean icmp4_ping_request(scan_info_t* s) {
  shiba_network_socket_t* sock = shiba_network_create_socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
  if (!sock) {
    fprintf(stderr, "Failed to create shiba ICMP socket in %s on line %d!\n", __FILE__, __LINE__);
    return FALSE;
  }

  s_icmp_hdr icmp = {
    .type = ICMP_ECHO,
    .code = 0,
    .checksum = 0,
    .id = 0,
    .sequence = 0
  };
  icmp.checksum = icmp_checksum((u16*)&icmp, sizeof(icmp));

  struct sockaddr_in dst_addr = {0};
  dst_addr.sin_family = s->af;
  dst_addr.sin_addr.s_addr = inet_addr(s->targets->target);

  struct timeval tv = {0};
  tv.tv_sec = 0;
  tv.tv_usec = s->timeout_ms;
  setsockopt(sock->handle, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

  int res = sendto(sock->handle, &icmp, sizeof(icmp), 0, (struct sockaddr*)&dst_addr, sizeof(dst_addr));
  if (res < 0) {
    fprintf(stderr, "sendto() failed in %s!\n", __FILE__);
    return FALSE;
  }

  printf("Sent %d bytes to %s!\n", res, s->targets->target);
  struct sockaddr_in client_addr = {0};
  socklen_t client_len = sizeof(client_addr);

  res = recvfrom(sock->handle, &icmp, sizeof(icmp), 0, (struct sockaddr*)&client_addr, &client_len);
  // TODO: Make this a verbose option later. But useful for now imo.
  if (res == -1) {
    fprintf(stderr, "Failed to receive any bytes from %s. Host is likely down.\n", s->targets->target);
    return FALSE;
  }

  printf("Received response from host %s!\n", s->targets->target);

  shiba_network_destroy_socket(sock);
  return TRUE;
}

