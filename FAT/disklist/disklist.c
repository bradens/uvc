/*
 * Part II
 * Programming Assignment 3, Csc 360
 * Braden Simpson, V00685500
 */
#include "disklist.h"
int main (int argc, const char * argv[]) {
    if (argc < 2) {
		printf("Usage: ./disklist [.img file]\n");
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
	fseek(infile, RootPtr * BlockSize, SEEK_CUR);					/* Move to the root directory	*/
	currentSegmentSize = 64;								/* Size of directory entry		*/
	currentPtr = malloc(currentSegmentSize);
	/* read in 1 directory entry at a time */
	int bytesRead = fread(currentPtr, 1, currentSegmentSize, infile);	
	while(1) {
		if (bytesRead >= RootCount*BlockSize) break;				/* max amount of files is RootCount		*/
		int check = printNode(currentPtr);					/* call printNode with the 64 bytes		*/
		if (check) {
			currentPtr += 63;								/* 63 because printNode already inc'd 1 */
		}
		/* read 64 more bytes	*/
		bytesRead += fread(currentPtr, 1, currentSegmentSize, infile);		
	}
	return 0;
}

/* This function reads the memory from <currentPtr> and sets		*
 * a DEntry object's values.  Then prints all the required info		*
 * in the format.  Returns 1 if there are no more entries left		*/
int printNode(void *currentPtr) {
	DEntry Entry; int i;
	/* Get the status byte, no need for ntohs because */
	memcpy(&Entry.status, currentPtr, 1);
	currentPtr++;
	
	if (!CHECK_BIT(Entry.status, 0))						/* checks if the bit in <pos> is 1 */
		return 1;
	
	/* Get the starting block	*/
	memcpy(&Entry.startBlock, currentPtr, 4);
	Entry.startBlock = ntohl(Entry.startBlock);
	currentPtr += 4;
		
	/*	No need for this right now, maybe later 
	 *  memcpy(&Entry.numBlocks, currentPtr, 4);
	 *	Entry.numBlocks = ntohl(Entry.numBlocks);
	 */	
	currentPtr += 4;										/* Don't need the number of blocks	*/
	
	memcpy(&Entry.fileSize, currentPtr, 4);
	Entry.fileSize = ntohl(Entry.fileSize);
	currentPtr += 4;
	currentPtr += 7;										/* Don't need the creation date		*/
	
	memcpy(&Entry.modifyTimeYear, currentPtr, 2);
	Entry.modifyTimeYear = ntohs(Entry.modifyTimeYear);		/* Get the year as a short int		*/
	currentPtr += 2;
	
	for (i = 0;i < 5;i++) {
		memcpy(&Entry.modifyTime[i], currentPtr, 1);		/* store the date as YYYYMMDDHHSS	*/
		currentPtr++;
	}	
	
	memcpy(Entry.fileName, currentPtr, 31);					/* Get filename from 31 bytes, its already  */
	currentPtr += 37;										/* null terminated. skip the unused bytes	*/
	/* Output nicely */
	if (CHECK_BIT(Entry.status, 1))
		printf("F");
	else
		printf("D");
		
	printf("%10d%30s %4d/", Entry.fileSize, Entry.fileName, Entry.modifyTimeYear);
	for (i = 0;i < 4;i++) {
		if (i == 0)
			printf("%.2d/", Entry.modifyTime[i]);
		else if (i == 1)
			printf("%.2d ", Entry.modifyTime[i]);
		else
			printf("%.2d:", Entry.modifyTime[i]);
	}
	printf("%.2d\n", Entry.modifyTime[4]);
	return 0;
}
