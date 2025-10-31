#include "./arg-parser.h"
#include "../libs/shiba-core/shiba.h"
#include "../libs/shiba-getopt/shiba-getopt.h"

int parse_args(int argc, char *argv[]) {
  if (argc <= 1) {
    print_usage();
    exit(1);
  }

  // uint16* port_list = NULL;
  int option_index = 0;
  int arg = 0;

  struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
    {0, 0, 0, 0}
  };

  while ((arg = getopt_long_only(argc, argv, "hv", long_options, &option_index)) != EOF) {
    switch (arg) {

      case '?':
      print_usage();
      exit(1);
      break;

      case 'h':
      print_usage();
      exit(0);
      break;

      case 'v':
      print_version();
      exit(0);
      break;

      default:
      fprintf(stderr, "smap -- Unknown argument: %c\n", arg);
      exit(1);
    }
  }

  return 0;
}

void print_version(void) {
  printf("smap version 0.1 -- made by ProfShiba\n");
  exit(0);
}

void print_usage(void) {
  printf("smap (shiba mapper)\nUsage: smap <FLAGS> <TARGET>\n");
  printf("\nOPTIONS:\n");
  printf("-h,  --help    Display this help message\n");
  printf("-v,  --version    Display the version information\n");
  printf("\nEXAMPLES:\n");
  printf("smap --help\n");
  printf("smap -v\n");
  // printf("smap -p 22,80,443,445 8.8.8.8\n");
}
