#include <math.h>
#include <stdio.h>
#include "sigmoid.h"

void sigmoid(double *n) { 
	*n = (1 / (1 + pow(EULER_NUMBER, -(*n))));
}

void sigmoidf(float *n){
	*n = (1 / (1 + pow(EULER_NUMBER_F, -(*n))));
}

void sigmoidl(__float128 *n){
	*n = (1 / (1 + powl(EULER_NUMBER_L, -(*n))));
}
