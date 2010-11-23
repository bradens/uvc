#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#define CHECK_BIT(var, pos) !!((var) & (1 << (pos)))

typedef struct directory_entry {
	char status;
	int startBlock;
	int numBlocks;
	int fileSize;
	char createTime[7];
	char modifyTime[7];
	char fileName[31];
	char unUsed[6];
} DEntry;


int FatTable;