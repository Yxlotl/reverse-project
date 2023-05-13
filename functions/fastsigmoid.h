#include <math.h>
#ifndef FSIGMOID_DEFS
#define FSIGMOID_DEFS

#define EXP_A (1048576/M_LN2)
#define EXP_C 60801
#define EXP(y) (_eco.n.i = EXP_A*(y) + (1072693248 - EXP_C), _eco.d)

double fastsigmoid(double n);
float fastsigmoidf(float n);
__float128 fastsigmoidl(__float128 n);

#endif
