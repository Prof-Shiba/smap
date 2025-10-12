#include "./libs/shiba-core/shiba.h"
#include "./libs/shiba-network/shiba-network.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
  printf("smap starting...\n\n");

  int32 port = -1;
  char* addr = "192.168.12.27";

  if (is_valid_port(port))
    printf("Valid port!\n");
  else
    printf("Invalid port!\n");

  if (is_valid_ipv4_address(addr))
    printf("valid ipv4 address!\n");
  else
    printf("Invalid ipv4 addr!\n");
}
