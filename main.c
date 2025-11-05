#include "./libs/shiba-core/shiba.h"
#include "./smap-options/smap-options.h"
#include "./libs/shiba-network/shiba-network.h"
#include "./libs/shiba-threads/shiba-threads.h"
#include "./arg-parsing/arg-parser.h"

// TODO: 1. -st TCP-Connect Scanning
// 2. -ss SYN scanning
// 2. -sf FIN scanning
// 2. -su UDP scanning
// 2. -sn ping scanning

int main(int argc, char *argv[]) {
  scan_info_t* s = calloc(1, sizeof(*s));
  if (!s) {
    fprintf(stderr, "Failed to allocate space for scan information!\n");
    return 1;
  }

  if (parse_args(argc, argv, s) != 0) {
    fprintf(stderr, "Failed to parse args!\n");
    return 1;
  }

  s->address = argv[argc - 1]; // we will validate this later, it might be some garbage input or none at all

  // TODO: Call network handling functions. Will validate address, scan ports, etc

  free(s);
}
