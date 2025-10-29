#include "./libs/shiba-core/shiba.h"
#include "./libs/shiba-network/shiba-network.h"
#include "./libs/shiba-threads/shiba-threads.h"
#include "./arg-parsing/arg-parser.h"


int main(int argc, char *argv[]) {
  if (parse_args(argc, argv) != 0) {
    fprintf(stderr, "Failed to parse args!\n"); // temp
    return 1;
  }
}
