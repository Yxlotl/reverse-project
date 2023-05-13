#include "benchmark.h"
#include "functions/sigmoid.c"
#include "functions/fastsigmoid.c"

int main(int argc, char* argv[]) {
	ResponseData r;
	parseInput(&r, argc, argv);
	printDebugMessage(&r);
	doOperations(&r);
}

//format
//<operation> <file1> <file2> <isDebug?> <operations> <type>
void parseInput(ResponseData* data, int argc, char* argv[]){
	if(argc < 2) printUseMessage("No arguments provided.");
	switch(argv[1][0]) {
		case 'm':
		case 'M':
		data->operation = 1;
		strcpy(data->opname, "Make a new file");
		break;
		case 'd':
		case 'D':
		data->operation = 2;
		strcpy(data->opname, "(Debug) Convert an output file to ascii");
		break;
		case 't':
		case 'T':
		data->operation = 3;
		strcpy(data->opname, "Test a benchmark");
		break;
		default:
		printUseMessage("Not a supported operation!");
	}
	
	if(data->operation == 1) {
		if(argc != 5) printUseMessage("Invalid arg count!");
		//check for argcount
		FILE* fp = fopen(argv[2], "wb");
		if(fp == NULL) printUseMessage("Could not open file for writing!");
		data->fileOut = fp;
		
		int N;
		N = atoi(argv[3]);
		if(N == 0) printUseMessage("Please choose an integer higher than 0!");
		data->N = N;
		
		switch(argv[4][0]) {
			case 'f':
			case 'F':
			data->type = 1;
			strcpy(data->strname, "float");
			break;
			case 'd':
			case 'D':
			data->type = 2;
			strcpy(data->strname, "double");
			break;
			case 'q':
			case 'Q':
			data->type = 3;
			strcpy(data->strname, "quad");
			break;
			default:
			printUseMessage("Invalid data type selected.");
		}
	}
	
	if(data->operation == 2) {
		if(argc != 4) printUseMessage("Invalid number of arguments!");
		FILE* wf = fopen(argv[3], "w");
		if(wf == NULL) printUseMessage("Could not open file for writing!");
		FILE* rf = fopen(argv[2], "rb");
		if(rf == NULL) printUseMessage("Could not open file for reading!");
		data->fileIn = rf;
		data->fileOut = wf;
	}
	
	if(data->operation == 3) {
		if(argc != 5) printUseMessage("Invalid number of arguments!");
		FILE* wf = fopen(argv[3], "wb");
		if(wf == NULL) printUseMessage("Could not open file for writing!");
		FILE* rf = fopen(argv[2], "rb");
		if(rf == NULL) printUseMessage("Could not open file for reading!");
		data->fileIn = rf;
		data->fileOut = wf;
		switch(argv[4][0]){
			case 's':
			case 'S':
			data->fast = 0;
			break;
			case 'f':
			case 'F':
			data->fast = 1;
			break;
		}
	}
}
void doOperations(ResponseData* data) {
	if(data->operation == 1) {
		char type;
		int count;
		float f;
		double d;
		__float128 q;
		type = data->type;
		count = data->N;
		__float128 delta = (__float128)(1)/(__float128)(count);
		fwrite(&type, sizeof type, 1, data->fileOut);
		fwrite(&count, sizeof count, 1, data->fileOut);
		for(int i = 0; i < count+1; i++) {
			if(type == 1) {
				f = (float)((float)(i)*delta);
				fwrite(&f, sizeof f, 1, data->fileOut);
				if(i < 5) printf("#%d: %1.8f\n", i, f); 
			}	
			else if(type == 2) {
				d = (double)((double)(i)*delta);
				fwrite(&d, sizeof d, 1, data->fileOut);
				if(i < 5) printf("#%d: %1.16f\n", i, d); 
			}
			else if(type == 3) {
				q = (__float128)((__float128)(i)*delta);
				fwrite(&q, sizeof q, 1, data->fileOut);
				if(i < 5) printf("#%d: %1.32Lf\n", i, (long double)q); 
			}
		}
		fclose(data->fileOut);
	}
	else if(data->operation == 2) {
		char type;
		fread(&type, sizeof type, 1, data->fileIn);
		
		char buff[20];
		if(type == 1) {
			strcpy(buff, "float");
		}
		if(type == 2) {
			strcpy(buff, "double");
		}
		if(type == 3) {
			strcpy(buff, "quad");
		}
		strcpy(data->strname, buff);
		//read the number of arguments
		int N;
		fread(&N, sizeof N, 1, data->fileIn);
		printf("Reading %d %ss from file and printing human-readable values in output file.", N, buff); 	
		fprintf(data->fileOut, "%s\nCOUNT %d\n", buff, N);

		float f;
		double d;
		__float128 q;
		for(int i = 0; i < N; i++) {
			if(type == 1) {
				fread(&f, sizeof f, 1, data->fileIn);
				fprintf(data->fileOut, "%1.8f\n", f);
			}	
			else if(type == 2) {
				fread(&d, sizeof d, 1, data->fileIn);
				fprintf(data->fileOut, "%1.16f\n", d);
			}
			else if(type == 3) {
				fread(&q, sizeof q, 1, data->fileIn);
				fprintf(data->fileOut, "%1.32Lf\n", (long double)q);
			}
		}
	} else if(data->operation == 3) {
		
		char type;
		fread(&type, sizeof type, 1, data->fileIn);
		
		char buff[20];
		if(type == 1) {
			strcpy(buff, "float");
		}
		if(type == 2) {
			strcpy(buff, "double");
		}
		if(type == 3) {
			strcpy(buff, "quad");
		}
		strcpy(data->strname, buff);
		//read the number of arguments
		int N;
		fread(&N, sizeof N, 1, data->fileIn);
		char fbuff[6];
		if(data->fast == 0) strcpy(fbuff, "slow");
		else strcpy(fbuff, "fast");
		printf("Read %d %ss from file; now transforming them by the %s sigmoid function.", N, buff, fbuff); 	
		
		
		
		__int64 freq, start, end, diff;
		
				
		float f;
		double d;
		__float128 q;
		
		fwrite(&type, sizeof type, 1, data->fileOut);
		fwrite(&N, sizeof N, 1, data->fileOut);
		// start
		QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		switch(data->fast) {
			case 0:
				switch(type){
					case 1:
						QueryPerformanceCounter((LARGE_INTEGER*)&start);
						for(int i = 0; i < N; i++){
							fread(&f, sizeof f, 1, data->fileIn);
							sigmoidf(&f);
							fwrite(&f, sizeof f, 1, data->fileOut);
						}
						QueryPerformanceCounter((LARGE_INTEGER*)&end);
					break;
					case 2:
						QueryPerformanceCounter((LARGE_INTEGER*)&start);
						for(int i = 0; i < N; i++){
							fread(&d, sizeof d, 1, data->fileIn);
							sigmoid(&d);
							fwrite(&d, sizeof d, 1, data->fileOut);
						}
						QueryPerformanceCounter((LARGE_INTEGER*)&end);
					break;
					case 3:
						QueryPerformanceCounter((LARGE_INTEGER*)&start);
						for(int i = 0; i < N; i++){
							fread(&q, sizeof q, 1, data->fileIn);
							sigmoidl(&q);
							fwrite(&q, sizeof q, 1, data->fileOut);
						}
						QueryPerformanceCounter((LARGE_INTEGER*)&end);
					break;
				}
			break;
			case 1:
				switch(type){
					case 1:
						QueryPerformanceCounter((LARGE_INTEGER*)&start);
						for(int i = 0; i < N; i++){
							fread(&f, sizeof f, 1, data->fileIn);
							f = (float) fastsigmoid((double) f);
							fwrite(&f, sizeof f, 1, data->fileOut);
						}
						QueryPerformanceCounter((LARGE_INTEGER*)&end);
					break;
					case 2:
						QueryPerformanceCounter((LARGE_INTEGER*)&start);
						for(int i = 0; i < N; i++){
							fread(&d, sizeof d, 1, data->fileIn);
							d = fastsigmoid(d);
							fwrite(&d, sizeof d, 1, data->fileOut);
						}
						QueryPerformanceCounter((LARGE_INTEGER*)&end);
					break;
					case 3:
						QueryPerformanceCounter((LARGE_INTEGER*)&start);
						for(int i = 0; i < N; i++){
							fread(&q, sizeof q, 1, data->fileIn);
							q = fastsigmoid((double)q);
							fwrite(&q, sizeof q, 1, data->fileOut);
						}
						QueryPerformanceCounter((LARGE_INTEGER*)&end);
					break;
				}			
			break;
		}

		diff = ((end - start) * 1000) / freq;
		
		unsigned int milliseconds = (unsigned int)(diff & 0xffffffff);
		printf("\nThe operation was performed in %u ms\n", milliseconds);
	}
}
void printUseMessage(const char* msg) {
	printf("ERROR: %s\nUse it like this instead: %s\n\n", msg, "<m, d, t>");
	printf("Use m to \'make\' a new file, d to \'debug\' a binary file and convert it to a text file, and t to \'transform\' a binary file by the sigmoid function.\n");
	printf("The make command is of the format \'m <filename> <amount of numbers> <type>\', where type can be f (float), d (double) or q (quad).\n");
	printf("This generates a binary file with the specified amount of numbers, evenly distributed between 0 and 1.\n\n");
	printf("The debug command is of the format \'d <binaryfile> <textfile>\'.\n");
	printf("This generates a text file containing the contents of the binary file in a human-readable format.\n\n");
	printf("The transform command is of the format \'t <inputfile> <outputfile> <s, f>\', where inputfile and outputfile are binary files.\n");
	printf("If \'f\' is selected, the program will use the fast sigmoid function on each value and calculate the elapsed time. If \'s\' is selected, it will use the standard sigmoid function instead.\n");
	exit(1);
}
void printDebugMessage(ResponseData* data){
	printf("========================\n");
	printf("Operation: %s\nType: %s\n", data->opname, data->strname);
	printf("========================\n");
}