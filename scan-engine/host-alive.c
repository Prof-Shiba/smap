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

boolean icmp4_ping_request(const scan_info_t* s, const char* host) {
  if (s == NULL || s->targets == NULL) return FALSE;

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
  dst_addr.sin_addr.s_addr = inet_addr(host);

  struct timeval tv = {0};
  tv.tv_sec = 0;
  tv.tv_usec = s->timeout_ms;
  setsockopt(sock->handle, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

  ssize_t res = sendto(sock->handle, &icmp, sizeof(icmp), 0, (struct sockaddr*)&dst_addr, sizeof(dst_addr));
  if (res < 0) {
    fprintf(stderr, "sendto() failed in %s!\n", __FILE__);
    return FALSE;
  }

  // TODO: Make this a verbose option later. But useful for now imo.
  printf("Sent %zd bytes to %s!\n", res, host);
  struct sockaddr_in client_addr = {0};
  socklen_t client_len = sizeof(client_addr);

  res = recvfrom(sock->handle, &icmp, sizeof(icmp), 0, (struct sockaddr*)&client_addr, &client_len);
  if (res == -1) {
  // TODO: Make this a verbose option later. But useful for now imo.
    fprintf(stderr, "Failed to receive any bytes from %s. Host is likely down.\n", host);
    return FALSE;
  }

  printf("Received response from host %s!\n", host);

  shiba_network_destroy_socket(sock);
  return TRUE;
}

void set_host_status(scan_info_t* s) {
  if (s == NULL || s->targets == NULL) {
    return;
  }
  target_t* head = s->targets;

  while (s->targets) {
    if (icmp4_ping_request(s, s->targets->target) == TRUE) {
      s->targets->is_host_alive = TRUE;
    }
    else {
      s->targets->is_host_alive = FALSE;
    }

    s->targets = s->targets->next;
  }
  s->targets = head;
}
