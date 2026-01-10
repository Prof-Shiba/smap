#include "./scan_report.h"

// FIXME: WE ARE ONLY ASSUMING 1 IP ADDR FOR NOW. FIX LATER
// FIXME: Print them in order of smallest to largest port
void print_report(const scan_info_t* s) {
  fflush(stdout);
  printf("smap scan results for: %s\n", s->targets->target);
  printf("Scanned: %d port(s)\t", s->num_ports_to_scan);
  printf("Ignored: %d port(s)\t", s->ignored_ports);
  printf("Found %d open port(s)\t", s->open_ports);
  printf("%d closed port(s)\n", s->closed_ports);
  printf("PORT:\tSTATE:\n");
 
  for (int i = 0; i < s->num_ports_to_scan; i++) {
    if (s->port_list[s->port_nums[i]].state == PORT_OPEN) {
      printf("%d\tOPEN", s->port_nums[i]);

      switch(s->scan_type) {
        case SCAN_TCP:
          printf("/TCP\n");
          break;

        case SCAN_SYN:
          printf("/SYN\n");
          break;

        case SCAN_UDP:
          printf("/UDP\n");
          break;

        case SCAN_FIN:
          printf("/FIN\n");
          break;

        case SCAN_PING:
          printf("/PING\n");
          break;

        default:
          printf("(DEBUG ME) Reached default case of switch statement in (%s)", __FILE__);
          break;
      }
    }
  }
}
