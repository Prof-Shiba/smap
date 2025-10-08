#include "./libs/shiba-core/shiba.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
  printf("smap starting...\n");
  uint32 test = 218437; 
  uint16 d = 2;

  printf("%d\n", test);
  printf("%d\n", d);

  uint8* mal;
  mal = malloc(sizeof(*mal));

  uint16* mal2;
  mal2 = malloc(sizeof(*mal2));

  free(mal2);
  shiba_memory_debug_print(0);
}
