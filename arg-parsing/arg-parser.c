#include "./arg-parser.h"
#include "../smap-options/smap-options.h"
#include "../libs/shiba-core/shiba.h"
#include "../libs/shiba-getopt/shiba-getopt.h"

// TODO: fill port list

int parse_args(int argc, char *argv[], scan_info_t* s) {
  if (argc <= 1) {
    print_usage(argv);
    exit(1);
  }

  int option_index = 0;
  int arg = 0;
  boolean is_root = check_if_root(); // we need elevated privs for packet manipulation

  struct option long_options[] = {
    // misc stuff
    {"help", NULL, no_argument, 'h'},
    {"version", NULL, no_argument, 'v'},

    // probe stuff
    {"port", NULL, required_argument, 'p'},

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
        s->scan_type = SCAN_TCP;
      else if (strcmp(opt_arg, "S") == 0)
        s->scan_type = SCAN_SYN;
      else if (strcmp(opt_arg, "U") == 0)
        s->scan_type = SCAN_UDP;
      else if (strcmp(opt_arg, "F") == 0)
        s->scan_type = SCAN_FIN;
      else if (strcmp(opt_arg, "N") == 0)
        s->scan_type = SCAN_PING;
      else {
        fprintf(stderr, "Unknown scan type: %s\n", opt_arg);
        exit(1);
      }
      if (s->scan_type == SCAN_SYN || s->scan_type == SCAN_UDP || s->scan_type == SCAN_FIN)
        if (is_root == FALSE) {
          fprintf(stderr, "Elevated privileges required to run a -s%s scan!\nTerminating due to insufficient privilege level!\n", opt_arg);
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
      fprintf(stderr, "smap -- Unknown argument: %c\n", arg);
      exit(1);
    }
  }

  return 0;
}

void print_version(char* argv[]) {
  printf("smap version 0.1 -- made by ProfShiba\n");
  exit(0);
}

void print_usage(char* argv[]) {
  printf("smap (shiba mapper)\nUsage: %s <FLAGS> <TARGET>\n", argv[0]);
  printf("\nOPTIONS:\n");
  printf("-h,  --help    Display this help message\n");
  printf("-v,  --version    Display the version information\n");
  printf("\nEXAMPLES:\n");
  printf("%s --help\n", argv[0]);
  printf("%s -v\n", argv[0]);
  printf("smap -p 22,80,443,445 8.8.8.8\n");
}
