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

void icmp4_ping_request(void) {
  shiba_network_socket_t* sock = shiba_network_create_socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
  if (!sock) {
    fprintf(stderr, "Failed to create shiba ICMP socket in %s on line %d!\n", __FILE__, __LINE__);
    return;
  }

  s_icmp_hdr icmp = {
    .type = ICMP_ECHO,
    .code = 0,
    .checksum = 0,
    .id = 0,
    .sequence = 0
  };
  icmp.checksum = icmp_checksum((u16*)&icmp, sizeof(icmp));
  

  shiba_network_destroy_socket(sock);
}

