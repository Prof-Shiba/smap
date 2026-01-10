#include "./arg-parser.h"
#include "../smap-options/smap-options.h"
#include "../libs/shiba-core/shiba.h"
#include "../libs/shiba-getopt/shiba-getopt.h"
#include "../port-engine/port-list.h"

// FIXME: When i add a random character to the end of the IP address,
// it displays every single port as being opened. IPv6 also does this.

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
    {"timeout", NULL, required_argument, 't'},

    // NULL TERM
    {0, 0, 0, 0}
  };

  while ((arg = getopt_long_only(argc, argv, "hvp:s:t:", long_options, &option_index)) != EOF) {
    switch (arg) {

      case '?':
        print_usage(argv);
        exit(1);
      break;

      case 'h':
        print_usage(argv);
        exit(0);
      break;

      // TODO: Prevent users from entering multiple scan types
      case 's':
        if (strcmp(opt_arg, "T") == 0) {
          s->scan_type = SCAN_TCP;
          s->sock_type = SOCK_STREAM;
        }
        else if (strcmp(opt_arg, "S") == 0) {
          s->scan_type = SCAN_SYN;
          s->sock_type = SOCK_RAW;
        }
        else if (strcmp(opt_arg, "U") == 0) {
          s->scan_type = SCAN_UDP;
          s->sock_type = SOCK_RAW;
        }
        else if (strcmp(opt_arg, "F") == 0) {
          s->scan_type = SCAN_FIN;
          s->sock_type = SOCK_RAW;
        }
        else if (strcmp(opt_arg, "N") == 0) {
          s->scan_type = SCAN_PING;
          // TODO: Need to find out what this will be.
        }
        else {
          shiba_fatal("Unknown scan type: %s", opt_arg);
        }
        if (s->scan_type == SCAN_SYN || s->scan_type == SCAN_UDP || s->scan_type == SCAN_FIN)
          if (is_root == FALSE) {
            shiba_fatal("FATAL: Elevated privileges required to run a -s%s scan!\nTerminating due to insufficient privilege level!", opt_arg);
        }
      break;

      case 'p':
        get_ports(opt_arg, s);
      break;

      case 'v':
        print_version(argv);
        exit(0);
      break;

      case 't':
        parse_timeout(opt_arg, s);
      break;

      default:
        shiba_fatal("smap -- default switch argument reached (%s). You entered: %c", __FILE__, arg);
    }
  }
  // the rest of the args are IP(s), so we parse them, make sure
  // its valid, then if valid add to linked list of IPs
  if (opt_index < argc) {
    link_ips(argc, argv, s);
  }
  else {
    shiba_fatal("FATAL: Target address not specified! Try smap -h");
  }

  return 0;
}

void print_version(char* argv[]) {
  printf("smap version 0.1 -- made by ProfShiba\n");
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
  printf("smap 8.8.8.8 -p-\n");
}

void parse_timeout(const char* timeout, scan_info_t* s) {
  char* endptr;
  u32 new_timeout;

  if (strlen(timeout) >= 10) {
    shiba_fatal("Your timeout value is unusually large. Try smap -h.\nTerminating.");
  }

  new_timeout = strtol(timeout, &endptr, 10);
  if (endptr == timeout || *endptr != '\0') {
    shiba_fatal("Invalid timeout parameter passed!\nTerminating.");
  }
  s->timeout = new_timeout;
}

void link_ips(int argc, char* argv[], scan_info_t* s) {
    target_t* last = NULL;

    while (opt_index < argc) {
      if (shiba_network_return_ip_type(argv[opt_index]) == 1) {
          shiba_fatal("Invalid IP Address! (%s)", argv[opt_index]);
      }

      if (!s->targets->target) {
          s->targets->target = argv[opt_index++];
      }
      else {
        target_t* current_target = malloc(sizeof(*current_target));
        current_target->target = argv[opt_index++];

        if (!s->targets->next)
          s->targets->next = current_target;
        else
          last->next = current_target;

        last = current_target;
      }
    }
}
