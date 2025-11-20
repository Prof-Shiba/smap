#include "./scan-engine.h"

// TODO: 
// 1. -ss SYN scanning
// 2. -sf FIN scanning
// 3. -su UDP scanning
// 4. -sn ping scanning

int scan_port(const scan_info_t* s, const uint16 port) {
  // we need to craft packets and get a socket
  // ready for each scan type
  switch(s->scan_type) {
    case SCAN_TCP:
      // TODO:
    break;

    case SCAN_SYN:
      // TODO:
    break;

    case SCAN_UDP:
      // TODO:
    break;

    case SCAN_FIN:
      // TODO:
    break;

    case SCAN_PING:
      // TODO:
    break;

    default:
    shiba_fatal("FATAL: Unknown scan type somehow passed to scan_ports in scan-engine.c!");
  }
}

void scan_ports(const scan_info_t* s) {
    // TODO: Loop through all ports and call scan_port
}
