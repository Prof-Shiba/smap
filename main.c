#include "./libs/shiba-core/shiba.h"
#include "./smap-options/smap-options.h"
#include "./libs/shiba-network/shiba-network.h"
#include "./libs/shiba-threads/shiba-threads.h"
#include "./arg-parsing/arg-parser.h"
#include "scan-engine/scan-engine.h"

int main(int argc, char *argv[]) {
  scan_info_t* s = malloc(sizeof(*s));
  if (!s) shiba_fatal("FATAL: Failed to allocate space for scan info! (main)");
  init_scan_info(s);

  // TODO: Get current time, show how long it took for program to finish

  if (parse_args(argc, argv, s) != 0)
    shiba_fatal("FATAL: Failed to parse args! (main)");

  // Im thinking, we have a general scan_ports function
  // which will check the scan type, and automatically call
  // the correct function to create a socket for it, then
  // calls an individual scan_port function in a loop?
  while (s->targets) {
    if (shiba_network_is_valid_ip_address(s->targets->target) == 4)
      printf("%s is a valid IPv4 Address!\n", s->targets->target);
    else if (shiba_network_is_valid_ip_address(s->targets->target) == 6)
      printf("%s is a valid IPv6 Address!\n", s->targets->target);
    s->targets = s->targets->next;
  }

  printf("Exiting successfully!\n");
  scan_info_cleanup(s);
}
