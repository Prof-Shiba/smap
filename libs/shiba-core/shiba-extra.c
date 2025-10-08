#include "shiba.h"

uint32 shiba_random_int(uint32 num) {
  num = (num << 13) ^ num;
  return ((num * (num * num * 15731 + 789221) + 1376312589) & 0x7fffffff);
}
