#include "./libs/shiba-core/shiba.h"
#include "./libs/shiba-network/shiba-network.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
  printf("smap starting...\n\n");

  char* addr = "110.128.59.255";
  int32 port = 65535;

  shiba_network_is_valid_ipv4_address(addr) ? printf("Valid IPv4 Address!\n") : printf("Invalid IPv4 Address!\n");
  shiba_network_is_valid_port(port) ? printf("Valid Port!\n") : printf("Invalid Port!\n");
}
