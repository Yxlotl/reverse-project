#ifndef DATASTREAM
#define DATASTREAM
#import <stdio.h>

extern void onEach(FILE* readFrom, FILE* sendTo, (void (*forEach(void* a));
extern FILE* openFile(const char* path, const char* mode, (void (*errorHandler())));
#endif