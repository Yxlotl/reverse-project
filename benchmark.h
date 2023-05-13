#include <stdio.h>
#include <unistd.h>
#include <profileapi.h>
#include <winnt.h>

typedef struct _ResponseData
{
	char operation, type, fast;
	int N;
	FILE *fileIn, *fileOut, *fileIn2;
	char strname[40];
	char opname[40];
	char outfileName[100];
}	ResponseData;

void parseInput(ResponseData* response, int argc, char* argv[]);
void printUseMessage(const char* msg);
void doOperations(ResponseData* input);
void printDebugMessage(ResponseData* data);