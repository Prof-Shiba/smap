#include "./scan_report.h"

// NOTE: The print report function will change as we optimize
// how the scan_info_t type looks and how we handle the port_list

// FIXME: WE ARE ONLY ASSUMING 1 IP ADDR FOR NOW. FIX LATER
// WE ALSO NEED TO TRACK WHICH PORTS WERE ACTUALLY SCANNED
// PORT LIST MUST BE VASTLY REWORKED ASAP
void print_report(const scan_info_t* s) {
  // We need to go through every target ptr, show which ports were open
  // alongside with scan type was used, (22/tcp), ignored ports, closed ports
  // num of scanned ports
  fflush(stdout);
  printf("==================================================\n");
  printf("smap scan results for: %s\n", s->targets->target);
  printf("Scanned: %d port(s)\n", s->num_ports);
  printf("Ignored: %d port(s)\n", s->ignored_ports);
  printf("%d closed port(s)\n", s->closed_ports);
  printf("==================================================\n\n");
  printf("PORT:\n");

  for (int i = 1; i <= MAX_PORT; i++) {
    if (s->port_list[i] == TRUE) {
      printf("%d\tOPEN\n", i);
    }
  }

}
