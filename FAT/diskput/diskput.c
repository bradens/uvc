/*
 * Part IV
 * Programming Assignment 3, Csc 360
 * Braden Simpson, V00685500
 */

#include "diskput.h";
int main (int argc, char**argv) {
	if (argc < 3) {
		printf("Usage: ./diskput [filename] [img file]\n");
		return 0;
	}
	infile = fopen(argv[1], "r");

	int BlockSize, StartPtr, RootPtr, RootCount, currentSegmentSize, block;
	currentSegmentSize = 2;
	void *currentPtr = malloc(currentSegmentSize);
	fseek(infile, 8, SEEK_CUR);			/* Skip past the name */
	/* block determines which part of the superblock to examine */
	/* Read Necessary parts of the superblock, can make NO assumptions */
	for (block = 0;block < 6;block++) {
		switch(block) {
			case 0:
				currentSegmentSize = 2;
				currentPtr = malloc(currentSegmentSize);
				fread(currentPtr, 1, currentSegmentSize, infile);
				memcpy(&BlockSize, currentPtr, currentSegmentSize);
				BlockSize = ntohs(BlockSize);
				break;
			case 1:
				currentSegmentSize = 4;
				fseek(infile, currentSegmentSize, SEEK_CUR);
				break;
			case 2:
				currentPtr = malloc(currentSegmentSize);
				fread(currentPtr, 1, currentSegmentSize, infile);
				memcpy(&StartPtr, currentPtr, currentSegmentSize);
				StartPtr = ntohl(StartPtr);
				break;
			case 3:
				fseek(infile, currentSegmentSize, SEEK_CUR);
				break;
			case 4:
				currentPtr = malloc(currentSegmentSize);
				fread(currentPtr, 1, currentSegmentSize, infile);
				memcpy(&RootPtr, currentPtr, currentSegmentSize);
				RootPtr = ntohl(RootPtr);
				break;
			case 5:
				currentPtr = malloc(currentSegmentSize);
				fread(currentPtr, 1, currentSegmentSize, infile);
				memcpy(&RootCount, currentPtr, currentSegmentSize);
				RootCount = ntohl(RootCount);
				break;
		}
		free(currentPtr);
		currentPtr = NULL;
	}

	/* Fill up the FatTable */
	rewind(infile);
	fseek(infile, 512 * StartPtr, SEEK_CUR);
	currentPtr = NULL;
	currentPtr = malloc(4);
	int hexVal, bytes, i;
	bytes = 0;
	for (i = 0;i < 6400;i++) {
		bytes += fread(currentPtr, 1, 4, infile);
		memcpy(&hexVal, currentPtr, 4);
		hexVal = ntohl(hexVal);
		FatTable[i] = hexVal;
		free(currentPtr);
		currentPtr = NULL;
		currentPtr = malloc(4);
	}

	/* Now search for an empty file */
	rewind(infile);
	fseek(infile, RootPtr * 512, SEEK_CUR);					/* Move to the root directory	*/
	currentSegmentSize = 64;								/* size Directory Entries		*/
	currentPtr = malloc(currentSegmentSize);
	/* read in 1 directory entry at a time */
	int bytesRead = fread(currentPtr, 1, currentSegmentSize, infile);
	while(1) {
		if (bytesRead >= RootCount*512) break;				/* gone through everything in root		*/
		if (IsEmptyNode(currentPtr)) {
			WriteToTestFS(currentPtr, StartPtr, argv[2]);
			return 0;
		}
		/* Read 64 more bytes */
		else {
				bytesRead += fread(currentPtr, 1, currentSegmentSize, infile);
		}
	}
	printf("No free space.");
	return 0;
}

/* This function reads the memory in <currentPtr> and	*
 * checks the file name for the desired file.			*/
int IsEmptyNode(void *currentPtr)
{
	/* Get the status */
	short Status;
	memcpy(&Status, currentPtr, 1);
	if (!CHECK_BIT(Status, 0)) {
		currentPtr--;					/* File is unused roll back pointer to start			*/
		return 1;
	}
	currentPtr--;						/* File is used roll back pointer to start			*/
	return 0;
}

int WriteToTestFS(void *currentPtr, int FatStart, char *ToWrite) {
	return 0;
}
