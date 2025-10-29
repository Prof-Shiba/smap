#include "./arg-parser.h"
#include "../libs/shiba-core/shiba.h"

// we need some struct here that can store information about
// flags and arguments they need

int parse_args(int argc, char *argv[]) {
  if (argc <= 1) {
    print_usage();
    exit(1);
  }

  return 0;
}

void print_version(void) {
  printf("smap version 0.1 - made by ProfShiba\n");
  exit(0);
}

void print_usage(void) {
  printf("smap\nUsage: smap <ip> <flags>\n\n");
  printf("Example: smap 8.8.8.8 -p 22,80,443,445\n");
}
