#import "datastream.h"
#import <stdio.h>

FILE* openFile(const char* path, const char* mode, (void (*errorHandler()))) {
	FILE* fp = fopen(path, mode);
	if(fp == NULL) errorHandler();
	else return fp;
}
void onEach(FILE* readFrom, FILE* sendTo, (void (*forEach(void* a)) {
	
}