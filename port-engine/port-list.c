#include "./port-list.h"

// NOTE: Formatted as 21,22,445 etc
int get_ports(char* ports, scan_info_t* s) {
  // How should we handle this? Take every char until a comma, and make it one uint16? total_ports++
  // Add to an array? Then getting the s->num_ports etc is easy

  // while port_list != '\0', if char != ',' then add num to string, keep doing until
  // we hit a comma, and set our index in the array back to 0, then make the array char an int and store
  // in a 65535 array of ints?

  uint16 arr_index = 0, total_ports = 0;
  char* temp = "";

  // special case, '-p-' means scan all ports
  if (strcmp(ports, "-") == 0) {
    for (int i = 1; i <= MAX_PORT; i++)
      s->port_list[i] = TRUE;

    s->num_ports = MAX_PORT;
    return 0;
  }

  return 0;
}
