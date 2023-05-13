#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <quadmath.h>


int printError() {
	printf("Invalid format!\n Use <file g> <file f> <n> to read floats, doubles, or longdoubles from file g and store them in file f.\n\n");
	return -1;
}

int main(int argc, char *argv[]){
	char type = 255;
	//check proper argument count
	if(argc != 3) {
		return printError();
	}
	FILE *wfp;
	FILE *rfp;
		
	rfp = fopen(argv[1], "rb");
	fseek(rfp, 0, SEEK_SET);
	
	if(rfp == NULL) {
		printf("%s", "Read file not found.\n");
		printError();
	}
	wfp = fopen(argv[2], "w");
	fseek(wfp, 0, SEEK_SET);	

	if(wfp == NULL) {
		printf("%s", "Write file not found.\n");
		printError();
	}
	
	//read the type value
	fread(&type, sizeof type, 1, rfp);
	char typeASCII[3];

	char buff[20];
	if(type == 255) {
		printf("%s\n", "Type not defined!");
		printError();
	}
	if(type == 1) {
		strcpy(buff, "float");
		strcpy(typeASCII, "f");
	}
	if(type == 2) {
		strcpy(buff, "double");
		strcpy(typeASCII, "d");
	}
	if(type == 3) {
		strcpy(buff, "quad");
		strcpy(typeASCII, "Lf");
	}
	
	//read the number of arguments
	int N;
	fread(&N, sizeof N, 1, rfp);
	
	printf("Reading %d %ss from file \"%s\" and printing values in file \"%s\".\n", N, buff, argv[1], argv[2]); 	

	fprintf(wfp, "%s\nCOUNT %d\n", buff, N);

	float f;
	double d;
	__float128 q;
	for(int i = 0; i < N; i++) {
		if(type == 1) {
			fread(&f, sizeof f, 1, rfp);
			fprintf(wfp, "%1.8f\n", f);
		}	
		else if(type == 2) {
			fread(&d, sizeof d, 1, rfp);
			fprintf(wfp, "%1.16f\n", d);
		}
		else if(type == 3) {
			fread(&q, sizeof q, 1, rfp);
			fprintf(wfp, "%1.32Lf\n", (long double)q);
		}
	}
	fclose(wfp);
	fclose(rfp);
	return 0;
}