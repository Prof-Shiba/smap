#include "./scan-engine.h"

// TODO: 
// 1. -ss SYN scanning
// 2. -sf FIN scanning
// 3. -su UDP scanning
// 4. -sn ping scanning

// We need to create a function (or some way) to make a socket
// for the correct scan type. I think in every function we 
// can just call a single create_shiba_socket function that
// returns the FD

void scan_ports(scan_info_t* s) {
  target_t* head = s->targets;

  // FIXME: This NEEDS to be heavily optimized fairly soon,
  // im just making sure the logic works as i expect currently.
  // We probably shouldn't be using an array here and blindly
  // iterating thru the entire thing when we may only have a 
  // handful of ports to scan. Find a better way ASAP!
  while (s->targets) {
    for (int i = 1; i <= MAX_PORT; i++) {
      if (s->port_list[i] == TRUE) {
        if (scan_port(s, i) == 0) {
          s->open_ports++;
        }
        else {
          s->closed_ports++;
        }
      }
    }
    s->targets = s->targets->next;
  }

  s->targets = head;
  set_ignored_ports(s);
}

int scan_port(scan_info_t* s, const u16 port) {
  switch(s->scan_type) {
    case SCAN_TCP:
      return open_tcp_connect(s, port);
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
    return 1;
  }
  return 0;
}

void set_ignored_ports(scan_info_t* s) {
  s->ignored_ports = MAX_PORT - s->num_ports;
}
