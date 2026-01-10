#include "./libs/shiba-core/shiba.h"
#include "./smap-options/smap-options.h"
#include "./libs/shiba-network/shiba-network.h"
#include "./libs/shiba-threads/shiba-threads.h"
#include "./arg-parsing/arg-parser.h"
#include "./scan-engine/scan-engine.h"
#include "./output/scan_report.h"
#include "./output/print_time.h"

// maybe we dont want this? Because we could only scan a handful of ports
// and maybe we just want to make as many threads for as many ports as there
// are, until we hit MAX_THREADS? In that case, should it even be managed here?
#define MAX_THREADS 50

int main(int argc, char *argv[]) {
  // shiba_memory_debug_init(NULL, NULL, NULL);

  scan_info_t* s = malloc(sizeof(*s));
  if (!s) {
    shiba_fatal("FATAL: Failed to allocate space for scan info! (main)");
  }

  shiba_network_init();
  init_scan_info(s);

  // TODO: Get current time, show how long it took for program to finish

  if (parse_args(argc, argv, s) != 0) {
    shiba_fatal("FATAL: Failed to parse args! (main)");
  }

  printf("Starting smap scan for %s on %s\n", s->targets->target, print_time());

  scan_ports(s);
  print_report(s);

  // if (shiba_memory_debug()) {
  //   fprintf(stderr, "Buffer overrun detected!\n");
  // }
  //
  // shiba_memory_debug_print(0);
  //
  // u32 leaked = shiba_memory_debug_mem_usage();
  // if (leaked > 0) {
  //   fprintf(stderr, "Memory leak detected: %u bytes unfreed!\n", leaked);
  // }

  scan_info_cleanup(s);
}
