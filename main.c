#include "functions/sigmoid.c"
#include "functions/fastsigmoid.c"
#include <stdio.h>

int main(int argc, char* argv[]) {
	double f = 1/(double)3;
	sigmoid(&f);
	
	double g = 1/(double)3;
	g = fastsigmoid(g);
}