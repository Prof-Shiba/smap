#include "./arg-parser.h"
#include "../smap-options/smap-options.h"
#include "../libs/shiba-core/shiba.h"
#include "../libs/shiba-getopt/shiba-getopt.h"

// TODO: 1. -st TCP-Connect Scanning
// 2. -ss SYN scanning
// 2. -sf FIN scanning
// 2. -su UDP scanning
// 2. -sn ping scanning
// 3. Specify Ports

int parse_args(int argc, char *argv[]) {
  if (argc <= 1) {
    print_usage(argv);
    exit(1);
  }

  // uint16* port_list = NULL;
  int option_index = 0;
  int arg = 0;
  scan_type_t scan_type = SCAN_TCP;
  boolean is_root = check_if_root();

  struct option long_options[] = {
    // misc stuff
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},

    // probe stuff
    {"port", required_argument, NULL, 'p'},

    // NULL TERM
    {0, 0, 0, 0}
  };

  while ((arg = getopt_long_only(argc, argv, "hvp:s:", long_options, &option_index)) != EOF) {
    switch (arg) {

      case '?':
      print_usage(argv);
      exit(1);
      break;

      case 'h':
      print_usage(argv);
      exit(0);
      break;

      case 's':
      if (strcmp(opt_arg, "T") == 0)
        scan_type = SCAN_TCP;
      else if (strcmp(opt_arg, "S") == 0)
        scan_type = SCAN_SYN;
      else if (strcmp(opt_arg, "U") == 0)
        scan_type = SCAN_UDP;
      else if (strcmp(opt_arg, "F") == 0)
        scan_type = SCAN_FIN;
      else if (strcmp(opt_arg, "N") == 0)
        scan_type = SCAN_PING;
      else {
        fprintf(stderr, "Unknown scan type: %s\n", opt_arg);
        exit(1);
      }
      break;

      case 'p':
      printf("TODO: Port stuff\n");
      break;

      case 'v':
      print_version(argv);
      exit(0);
      break;

      default:
      fprintf(stderr, "%s -- Unknown argument: %c\n", argv[0], arg);
      exit(1);
    }
  }

  return 0;
}

void print_version(char* argv[]) {
  printf("%s version 0.1 -- made by ProfShiba\n", argv[0]);
  exit(0);
}

void print_usage(char* argv[]) {
  printf("%s (shiba mapper)\nUsage: %s <FLAGS> <TARGET>\n", argv[0], argv[0]);
  printf("\nOPTIONS:\n");
  printf("-h,  --help    Display this help message\n");
  printf("-v,  --version    Display the version information\n");
  printf("\nEXAMPLES:\n");
  printf("%s --help\n", argv[0]);
  printf("%s -v\n", argv[0]);
  printf("smap -p 22,80,443,445 8.8.8.8\n");
}
