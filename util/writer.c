#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <quadmath.h>

int printError() {
	printf("Invalid format!\n Use <n> <file> <-f, -d, -q> to generate n floats, doubles, or quads and store them in file f.\n\n");
	return -1;
}

int main(int argc, char *argv[]){
	char type = 255;
	//check proper argument count
	if(argc != 4) {
		return printError();
	}
	
	//check the formatting for the number of numbers to generate
	int N = atoi(argv[1]);
	if(N == 0) return printError();
	
	char buff[20];
	if(argv[3][1] == 'f') {
		type = 1;
		strcpy(buff, "float");
	}
	if(argv[3][1] == 'd') {
		type = 2;
		strcpy(buff, "double");
	}
	if(argv[3][1] == 'q') {
		type = 3;
		strcpy(buff, "quad");
	}

	printf("Creating %d distributed %ss in file \"%s\" between 0 and 1.\n", N, buff, argv[2]); 	
	
	FILE *fp;	
	fp = fopen(argv[2], "wb");
	if(fp == NULL) printError();
		
	fwrite(&type, sizeof type, 1, fp);
	fwrite(&N, sizeof N, 1, fp);

	float f;
	double d;
	__float128 q;
	__float128 delta = (__float128)(1)/(__float128)(N);
	for(int i = 0; i < N+1; i++) {
		if(type == 1) {
			f = (float)((float)(i)*delta);
			fwrite(&f, sizeof f, 1, fp);
			if(i < 5) printf("#%d: %1.8f\n", i, f); 
		}	
		else if(type == 2) {
			d = (double)((double)(i)*delta);
			fwrite(&d, sizeof d, 1, fp);
			if(i < 5) printf("#%d: %1.16f\n", i, d); 
		}
		else if(type == 3) {
			q = (__float128)((__float128)(i)*delta);
			fwrite(&q, sizeof q, 1, fp);
			if(i < 5) printf("#%d: %1.32Lf\n", i, (long double)q); 
		}
	}
	fclose(fp);
	return 0;
}
