#include "./scan-engine.h"
#include "host-alive.h"

void scan_ports(scan_info_t* s) {
  if (s == NULL || s->targets == NULL) return;

  target_t* head = s->targets;

  while (s->targets) {
    if (s->targets->is_host_alive == TRUE) {
      for (int i = 0; i < s->num_ports_to_scan; i++) {
        if (scan_port(s, s->port_nums[i]) == 0) {
          s->targets->port_list[s->port_nums[i]].state = PORT_OPEN;
          s->targets->open_ports++;
        }
        else {
          s->targets->port_list[s->port_nums[i]].state = PORT_CLOSED;
          s->targets->closed_ports++;
        }
      }
    }
    s->targets = s->targets->next;
  }

  s->targets = head;
}

int scan_port(scan_info_t* s, const u16 port) {
  if (s == NULL) return 1;

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
  if (s == NULL || s->targets == NULL) return;

  target_t* head = s->targets;

  while (s->targets) {
    s->targets->ignored_ports = MAX_PORT - s->num_ports_to_scan;
    s->targets = s->targets->next;
  }

  s->targets = head;
}
