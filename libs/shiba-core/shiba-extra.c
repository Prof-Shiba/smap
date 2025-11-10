#include "shiba.h"

uint32 shiba_random_int(uint32 num) {
  num = (num << 13) ^ num;
  return ((num * (num * num * 15731 + 789221) + 1376312589) & 0x7fffffff);
}

fint32 shiba_fast_inv_square_root(fint32 number) {
  int64 i = 0;
	fint32 x2 = 0, y = 0;
	const fint32 threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;
	i  = 0x5f3759df - ( i >> 1 );
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );

	return y;
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
