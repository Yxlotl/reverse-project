#include <stdio.h>
#include "fastsigmoid.h"

static union
{
	double d;
	struct
	{
	#ifdef LITTLE_ENDIAN
		int j, i;
	#else
		int i, j;
	#endif
	} n;
} _eco;

double fastsigmoid(double n) { 
	return (1 / (1 + EXP(-n)));
}
