#include "./libs/shiba-core/shiba.h"
#include "./smap-options/smap-options.h"
#include "./libs/shiba-network/shiba-network.h"
#include "./libs/shiba-threads/shiba-threads.h"
#include "./arg-parsing/arg-parser.h"
#include "./scan-engine/scan-engine.h"
#include "./output/scan_report.h"

// TODO: Refactor any code that I can to make it generally nice to work with
// PING the host(s) first to confirm they are up.
// Add DNS lookup
// After that, we search for any bugs we can find (eg. parsing bugs)
// Double check to see if anything needs a refactor, such as output arg parser
// then when everythings good, we add SYN stealth scanning using raw packets

int main(int argc, char *argv[]) {
  // shiba_memory_debug_init(NULL, NULL, NULL);
  clock_t start, end;
  f32 cpu_time;

  scan_info_t* s = malloc(sizeof(*s));
  if (!s) {
    shiba_fatal("FATAL: Failed to allocate space for scan info! (main)");
  }

  shiba_network_init();
  init_scan_info(s);

  if (parse_args(argc, argv, s) != 0) {
    shiba_fatal("FATAL: Failed to parse args! (main)");
  }

  (s->num_targets == 1) ?
    printf("Starting smap scan for 1 target on %s", print_time()) :
    printf("Starting smap scan for %d targets on %s", s->num_targets, print_time());

  start = clock();
  scan_ports(s);
  end = clock();

  cpu_time = ((f32) (end - start)) / CLOCKS_PER_SEC;
  set_ignored_ports(s);
  handle_report(s, cpu_time);

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
