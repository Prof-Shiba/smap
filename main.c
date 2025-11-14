#include "./libs/shiba-core/shiba.h"
#include "./smap-options/smap-options.h"
#include "./libs/shiba-network/shiba-network.h"
#include "./libs/shiba-threads/shiba-threads.h"
#include "./arg-parsing/arg-parser.h"

int main(int argc, char *argv[]) {
  scan_info_t* s = malloc(sizeof(*s));
  if (!s) shiba_fatal("FATAL: Failed to allocate space for scan info! (main)");
  init_scan_info(s);

  // TODO: Get current time, show how long it took for program to finish

  if (parse_args(argc, argv, s) != 0)
    shiba_fatal("FATAL: Failed to parse args! (main)");

  // TODO: Call network handling functions. Will validate address, scan ports, etc

  scan_info_cleanup(s);
}
