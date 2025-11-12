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
  scan_info_t* s = malloc(sizeof(*s));
  if (!s) shiba_fatal("Failed to create scan info!");
  init_scan_info(s);

  // TODO: Get current time, show how long it took for program to finish

  if (parse_args(argc, argv, s) != 0)
    shiba_fatal("Failed to parse args!");

  for (int i = 1; i <= MAX_PORT; i++) {
    if (s->port_list[i] == TRUE)
      printf("Port %d is marked for scanning!\n", i);
    else
      s->ignored_ports++;
  }
  printf("%d ports ignored for scanning!\n", s->ignored_ports);
  printf("%d ports being scanned!\n", s->num_ports);

  // TODO: Call network handling functions. Will validate address, scan ports, etc

  // TODO: one cleanup function that handles all freeing?
  free(s);
}
