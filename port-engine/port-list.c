#include "./port-list.h"

// FIXME: -p- displays random shit in unpredictable amounts

int get_ports(const char* ports, scan_info_t* s) {
  u16 j = 0;
  u16 arr_index = 0;
  i64 current_port = 0;
  char temp[8] = "";
  char* endptr;

  // special case, '-p-' means scan all ports
  if (strcmp(ports, "-") == 0) {
    s->port_max_size = MAX_PORT;

    u32* tmp = realloc(s->port_nums, sizeof(*s->port_nums) * (s->port_max_size + 1));
    if (!tmp)
      shiba_fatal("Realloc failed for port nums! (%s)", __FILE_NAME__);

    s->port_nums = tmp;

    for (int i = 1; i <= MAX_PORT; i++)
      s->port_nums[i] = i;

    s->num_ports_to_scan = MAX_PORT;
    return 0;
  }
  // TODO: add check here later for a -, indicating a range of ports to scan
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
        if (s->port_list[current_port].port_num == 0) {
          s->num_ports_to_scan++;
          s->port_list[current_port].port_num = current_port; // NOTE: storage
          s->port_nums[j++] = current_port;

          if (j >= s->port_max_size) {
            s->port_max_size *= 2;
            // this may be an issue because the extra spots are not zero'd out
            u32* tmp = realloc(s->port_nums, s->port_max_size * sizeof(*s->port_nums));
            if (!tmp)
              shiba_fatal("FATAL: Realloc failed on port nums! (%s)", __FILE_NAME__);

            // NOTE: Debug this to verify it does whats intended
            for (int i = s->port_max_size / 2; i < s->port_max_size; i++) {
              tmp[i] = 0;
            }

            s->port_nums = tmp;
          }
        }
        else {
          // NOTE: might be better to have a dupe flag or something so we dont spam messages for the same port being duped.
          printf("You've entered that port (%ld) already! Pay attention, you might bring a whole network down!\n", current_port);
        }
        memset(temp, 0, sizeof(temp));
        arr_index = 0;
      }
    }
  }

  return 0;
}
