#include "./scan-engine.h"

// TODO: 
// 1. -ss SYN scanning
// 2. -sf FIN scanning
// 3. -su UDP scanning
// 4. -sn ping scanning
// TODO: Filtered connections do not show as open

void scan_ports(scan_info_t* s) {
  target_t* head = s->targets;

  while (s->targets) {
    for (int i = 0; i < s->num_ports_to_scan; i++) {// NOTE: Double check this stuff
      if (scan_port(s, s->port_nums[i]) == 0) {
        s->targets->port_list[s->port_nums[i]].state = PORT_OPEN;
        s->targets->port_list->open_ports++;
      }
      else {
        s->targets->port_list[s->port_nums[i]].state = PORT_CLOSED;
        s->targets->port_list->closed_ports++;
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
  s->targets->port_list->ignored_ports = MAX_PORT - s->num_ports_to_scan;
}
