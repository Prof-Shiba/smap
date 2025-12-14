#include "./scan-engine.h"

// TODO: 
// 1. -ss SYN scanning
// 2. -sf FIN scanning
// 3. -su UDP scanning
// 4. -sn ping scanning

// FIXME: When i add a random character to the end of the IP address,
// it displays every single port as being opened. 
// smap -p- 172.31.11.195c does this. Also doesnt scan port 65535

void scan_ports(scan_info_t* s) {
  target_t* head = s->targets;

  while (s->targets) {
    // TODO: because it's per IP, I think we need some way to connect
    // the output messages, or make a backlog or something.
    // i dont really want scan_ports.c calling the print output messages.
    for (int i = 0; i < s->num_ports_to_scan; i++) {
      if (scan_port(s, s->port_nums[i]) == 0) {
        s->port_list[s->port_nums[i]].state = PORT_OPEN;
        s->open_ports++;
      }
      else {
        s->port_list[s->port_nums[i]].state = PORT_CLOSED;
        s->closed_ports++;
      }
    }
    s->targets = s->targets->next;
  }

  s->targets = head;
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
  s->ignored_ports = MAX_PORT - s->num_ports_to_scan;
}
