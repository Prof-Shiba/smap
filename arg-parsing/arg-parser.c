#include "./arg-parser.h"
#include "../libs/shiba-core/shiba.h"

// TODO: We will start by just making sure we can print the usage function
// with '-h' or '--help' and expand from there

int parse_args(int argc, char *argv[]) {
  if (argc <= 1) {
    print_usage();
    exit(1);
  }

  // custom getopt_long_only function that works on both windows and linux
  // looks like while((arg = getopt_long_only(argc, argv, shortopts, long_options, &option_index)) != EOF) {
  //  switch (arg) {
  //    
  //  }
  // }

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
