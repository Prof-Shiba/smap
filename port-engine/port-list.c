#include "./port-list.h"

int get_ports(const char* ports, scan_info_t* s) {
  uint16 arr_index = 0;
  int64 current_port = 0;
  char temp[8] = "";
  char* endptr;

  // special case, '-p-' means scan all ports
  if (strcmp(ports, "-") == 0) {
    for (int i = 1; i <= MAX_PORT; i++)
      s->port_list[i] = TRUE;

    s->num_ports = MAX_PORT;
    return 0;
  }
  else {
    for (int i = 0; i <= strlen(ports); i++) {
      if (ports[i] != ',' && ports[i] != '\0') {
        if (arr_index >= 6)
          error_invalid_port();
        temp[arr_index++] = ports[i];
      }
      else {
        current_port = strtol(temp, &endptr, 10);

        if (endptr == ports || *endptr != '\0'
          || current_port <= 0 || current_port > MAX_PORT) {
          error_invalid_port();
        }

        // TODO: Check for duplicate ports
        s->num_ports++;
        s->port_list[current_port] = TRUE;
        memset(temp, 0, sizeof(temp));
        arr_index = 0;

      }
    }
  }

  return 0;
}

void error_invalid_port(void) {
    fprintf(stderr, "Invalid port passed to -p! Ports must be an integer between 1-65535!\n");
    exit(1);
}
