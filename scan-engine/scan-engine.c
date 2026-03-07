#include "./scan-engine.h"

void scan_ports(scan_info_t* s) {
  target_t* head = s->targets;

  while (s->targets) {
    // NOTE: Temp for testing. We need to ping host, verify its alive,
    // and then that functions return res will determine if we set
    // to alive or not (not handled here)
    s->targets->is_host_alive = TRUE;

    if (s->targets->is_host_alive == TRUE) {
      for (int i = 0; i < s->num_ports_to_scan; i++) {
        // NOTE: i think its less work if the function called by scan_port 
        // checks if the host is alive and returns -1 if not, right? but if
        // we do that, then it limits our options for determing if the host is up
        // eg. maybe a ICMP packet gets dropped due to firewall, but trying from
        // port 53 works and we can determine the servers up. so i think
        // i should make a separate function which will independently determine if the
        // host is alive/dead, then we set targets->is_host_alive based on that
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
  target_t* head = s->targets;

  while (s->targets) {
    s->targets->ignored_ports = MAX_PORT - s->num_ports_to_scan;
    s->targets = s->targets->next;
  }

  s->targets = head;
}
