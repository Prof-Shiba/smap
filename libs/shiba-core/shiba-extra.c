#include "shiba.h"

u32 shiba_random_int(u32 num) {
  num = (num << 13) ^ num;
  return ((num * (num * num * 15731 + 789221) + 1376312589) & 0x7fffffff);
}

void shiba_fatal(const char* message, ...) {
	va_list args;
	fflush(stdout);
	fflush(stderr);

	va_start(args, message);
	vfprintf(stderr, message, args);
	va_end(args);

	fprintf(stderr, "\n");
	exit(1);
}
