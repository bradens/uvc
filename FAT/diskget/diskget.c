/*
 * Part III
 * Programming Assignment 3, Csc 360
 * Braden Simpson, V00685500
 */
#include "diskget.h";
int main(int argc, char **argv)
{
	if (argc < 3) {
		printf("Usage: ./diskget [.img file] [filename]\n");
		return 0;
	}
	FILE *infile = fopen(argv[1], "r");

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
	rewind(infile);
	fseek(infile, RootPtr * 512, SEEK_CUR);					/* Move to the root directory	*/
	currentSegmentSize = 64;								/* size Directory Entries		*/
	currentPtr = malloc(currentSegmentSize);
	/* read in 1 directory entry at a time */
	int bytesRead = fread(currentPtr, 1, currentSegmentSize, infile);
	while(1) {
		if (bytesRead >= RootCount*512) break;				/* gone through everything in root		*/
		if (IsCorrectNode(currentPtr, argv[2])) {					/* check if the desired node			*/
			currentPtr -= 64;								/* roll back pointer to start			*/
			WriteToLocalFS(currentPtr);
			break;
		}
		/* read 64 more bytes	*/
		else {
			bytesRead += fread(currentPtr, 1, currentSegmentSize, infile);
		}
	}
	return 0;
}

/* This function reads the memory in <currentPtr> and	*
 * checks the file name for the desired file.			*/
int IsCorrectNode(void *currentPtr, char *inString)
{
	char *filename = (char*)malloc(31);
	/* skip to the filename */
	currentPtr += 27;
	memcpy(filename, currentPtr, 31);
	currentPtr += 37;										/* skip the unused bytes 		*/
	if (strcmp(inString, filename) == 0) {
		return 1;
	}
	return 0;
}

int WriteToLocalFS(void *currentPtr, char *inString)
{
	FILE *outfile = fopen(inString, "w");
	int buf[128];
	DEntry Entry; int i;
	/* Get the status byte, no need for ntohs because */
	memcpy(&Entry.status, currentPtr, 1);
	currentPtr++;

	if (!CHECK_BIT(Entry.status, 0))						/* checks if the bit in <pos> is 1	*/
		return 1;											/* ERR								*/

	/* Get the starting block	*/
	memcpy(&Entry.startBlock, currentPtr, 4);
	Entry.startBlock = ntohl(Entry.startBlock);
	currentPtr += 4;

	/* TODO FIND THE FAT ENTRY THEN COPY THE FILE */

	return 0;
}
