// NOTE: A lot of stuff will be added before we add multi-threading,
// i dont want to add broken stuff that somehow conflicts with each other
// or fails outright under [x] circumstance, and then throw threading
// on top of it to make everything worse and harder to debug!


// TODO: Fix the port_list and generally the scan_info_t type ASAP!
// print_report really highlights how fucking awfully this is designed!

#include "./libs/shiba-core/shiba.h"
#include "./smap-options/smap-options.h"
#include "./libs/shiba-network/shiba-network.h"
#include "./libs/shiba-threads/shiba-threads.h"
#include "./arg-parsing/arg-parser.h"
#include "scan-engine/scan-engine.h"
#include "./output/scan_report.h"

int main(int argc, char *argv[]) {
  scan_info_t* s = malloc(sizeof(*s));
  if (!s) shiba_fatal("FATAL: Failed to allocate space for scan info! (main)");
  init_scan_info(s);

  // TODO: Get current time, show how long it took for program to finish

  if (parse_args(argc, argv, s) != 0)
    shiba_fatal("FATAL: Failed to parse args! (main)");

  // this stuff will be removed when windows testing is verified
  // target_t* head = s->targets; 
  // while (s->targets) {
  //   if (shiba_network_return_ip_type(s->targets->target) == 4)
  //     printf("%s is a valid IPv4 Address!\n", s->targets->target);
  //   else if (shiba_network_return_ip_type(s->targets->target) == 6)
  //     printf("%s is a valid IPv6 Address!\n", s->targets->target);
  //   s->targets = s->targets->next;
  // }
  // s->targets = head;

  scan_ports(s);
  print_report(s);

  printf("Exiting successfully!\n");
  scan_info_cleanup(s);
}
