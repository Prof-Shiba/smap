#include "shiba.h"

uint32 shiba_random_int(uint32 num) {
  num = (num << 13) ^ num;
  return ((num * (num * num * 15731 + 789221) + 1376312589) & 0x7fffffff);
}

fint32 shiba_fast_inv_square_root(fint32 number) {
  int64 i;
	fint32 x2, y;
	const fint32 threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;
	i  = 0x5f3759df - ( i >> 1 );
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );

	return y;
}
