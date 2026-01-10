#include "./print_time.h"

const char* print_time() {
  struct tm* local;
  time_t t = time(NULL);
  local = localtime(&t);

  return (asctime(local));
}
