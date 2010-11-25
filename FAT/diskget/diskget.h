/*
 * diskget.h
 *
 *  Created on: Nov 24, 2010
 *      Author: braden
 */
#include <stdio.h>;
#include <stdlib.h>;
#include <string.h>;
#include <arpa/inet.h>

#define CHECK_BIT(var, pos) !!((var) & (1 << (pos)))

/* The struct for a directory entry		*
 * all strings have 1 extra character	*
 * reserved for the '\0' character.		*/
typedef struct directory_entry {
	char status;
	int startBlock;
	int numBlocks;
	int fileSize;
	short createTimeYear;
	char createTime[6];
	short modifyTimeYear;
	char modifyTime[6];
	char fileName[31];
	char unUsed[6];
} DEntry;

FILE *infile;
unsigned int FatTable[6400];
int IsCorrectNode(void *currentPtr,char *inString);
int WriteToLocalFS(void *currentPtr, char *inString, int FatStart);