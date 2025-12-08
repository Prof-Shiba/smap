#include "./port-list.h"

// FIXME: If i manually put port 8000, it doesnt find it
// -p- WILL find it, but it will display random shit too
// This makes me think it might be 2 problems. Ones a port initialization
// or buffer overrun issue, the others a actual port scanning function issue

int get_ports(const char* ports, scan_info_t* s) {
  u16 arr_index = 0;
  i64 current_port = 0;
  char temp[8] = "";
  char* endptr;

  // special case, '-p-' means scan all ports
  if (strcmp(ports, "-") == 0) {
    s->port_nums = realloc(s->port_nums, sizeof(*s->port_nums) * (MAX_PORT + 1));
    if (!s->port_nums)
      shiba_fatal("Realloc failed for port nums! (%s)", __FILE_NAME__);

    // FIXME: This isnt doing anything
    for (int i = 1; i <= MAX_PORT; i++)
      s->port_nums[i] = i;

    s->num_ports_to_scan = MAX_PORT;
    return 0;
  }
  else {
    for (int i = 0; i <= strlen(ports); i++) {
      if (ports[i] != ',' && ports[i] != '\0') {
        if (arr_index >= 6)
          shiba_fatal("Invalid port passed to -p! Ports must be an integer between 1-65535");
        temp[arr_index++] = ports[i];
      }
      else {
        current_port = strtol(temp, &endptr, 10);

        if (endptr == ports || *endptr != '\0'
          || current_port <= 0 || current_port > MAX_PORT) {
          shiba_fatal("Invalid port passed to -p! Ports must be an integer between 1-65535");
        }
        // FIXME: Check if port_nums is too small for the current capacity
        if (s->port_list[current_port].port_num == 0) {
          s->num_ports_to_scan++;
          s->port_list[current_port].port_num = current_port;
        }
        else {
          printf("You've entered that port (%ld) already! Pay attention, you might bring a whole network down!\n", current_port);
        }
        memset(temp, 0, sizeof(temp));
        arr_index = 0;
      }
    }
  }

  return 0;
}
