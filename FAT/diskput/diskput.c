/*
 * Part IV
 * Programming Assignment 3, Csc 360
 * Braden Simpson, V00685500
 */

#include "diskput.h"
int main (int argc, char**argv) {
	if (argc < 3) {
		printf("Usage: ./diskput [filename] [img file]\n");
		return 0;
	}
	infile = fopen(argv[2], "r+");

	currentSegmentSize = 2; fileNum = 0;
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
				currentPtr = malloc(currentSegmentSize);
				fread(currentPtr, 1, currentSegmentSize, infile);
				memcpy(&FSCount, currentPtr, currentSegmentSize);
				FSCount = ntohl(FSCount);
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

	FatTable = (unsigned int *)malloc(sizeof(unsigned int) * FSCount);
	/* Fill up the FatTable */
	rewind(infile);
	fseek(infile, BlockSize * StartPtr, SEEK_CUR);
	currentPtr = NULL;
	currentPtr = malloc(4);
	int hexVal, bytes, i;
	bytes = 0;
	for (i = 0;i < FSCount;i++) {
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
	fseek(infile, RootPtr * BlockSize, SEEK_CUR);				/* Move to the root directory	*/
	currentSegmentSize = 64;						/* size Directory Entries	*/
	currentPtr = malloc(currentSegmentSize);
	/* read in 1 directory entry at a time */
	int bytesRead = fread(currentPtr, 1, currentSegmentSize, infile);
	while(1) {
		if (bytesRead >= RootCount*BlockSize) break;			/* gone through everything in root  */
		if (IsEmptyNode(currentPtr)) {
			WriteToTestFS(currentPtr, StartPtr, argv[1]);
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
		currentPtr--;				/* File is unused roll back pointer to start			*/
		return 1;
	}
	currentPtr--;	
	fileNum++;					/* File is used roll back pointer to start	*/
	return 0;
}

int WriteToTestFS(void *currentPtr, int FatStart, char *ToWrite) {
	FILE *ToCopy = fopen(ToWrite, "r");	
	DEntry fileEntry, fileEntryBigE;
	char buf[BlockSize];
	int i, currentBlock, hexVal, hexValBigE;
	hexVal = 0xffffffff;
	
	fileEntry.status = 3;					/* Set the entry status to 11					*/
	for (i = 0;;i++) {
		if (FatTable[i] == 0) 
			break;					/* FatTable[i] is free						*/
	} 
	fileEntry.startBlock = i;
	strcpy(fileEntry.fileName, ToWrite);
	rewind(infile);
	fseek(infile, BlockSize * i, SEEK_CUR);

	int bytesRead = fread(buf, 1, BlockSize, ToCopy);
	fwrite(buf, 1, bytesRead, infile);
	fileEntry.numBlocks = 1;
	fileEntry.fileSize = bytesRead;
	while(bytesRead > 0) {
		bytesRead = fread(buf, 1, BlockSize, ToCopy);
		if (bytesRead == 0) {
			/* Fill in ith fat table with EOF */
			rewind(infile);
			fseek(infile, BlockSize * FatStart, SEEK_CUR);
			fseek(infile, 4 * i, SEEK_CUR);
			hexValBigE = ntohl(hexVal);
			fwrite(&hexValBigE, 1, 4, infile);
			FatTable[i] = hexVal;	
			break;
		}
		currentBlock = i;
		i++;
		for (;;i++) {
			if (i == FSCount) {
				printf("File too large for remaining space, terminating.  Image may be corrupted.\n");
				return 1;
			}
			if (FatTable[i] == 0) 
				break;					/* FatTable[i] is free			*/
		} 
		
		/* Fill in current fat table entry with next available FAT spot */
		rewind(infile);
		fseek(infile, BlockSize * FatStart, SEEK_CUR);
		fseek(infile, 4 * currentBlock, SEEK_CUR);
		fwrite(&i, 1, 4, infile);
		FatTable[currentBlock] = i;
		
		/* Now seek to the free fat spot just found and write buf to it */
		/* Write "BlockSize" bytes to the free block */
		rewind(infile);
		fseek(infile, BlockSize * i, SEEK_CUR);
		fwrite(buf, 1, bytesRead, infile);
		fileEntry.fileSize += bytesRead;
		fileEntry.numBlocks++;
	}
	
	rewind(infile);
	fseek(infile, BlockSize * RootPtr, SEEK_CUR);
	fseek(infile, 64 * fileNum, SEEK_CUR);
	//fileEntry.fileSize = BlockSize * fileEntry.numBlocks;
	time_t rawTime;
	time(&rawTime);
	struct tm *currentTime = localtime(&rawTime);
	
	fileEntry.createTimeYear = (short)(currentTime->tm_year) + 1900;
	fileEntry.createTime[0] = (char)(currentTime->tm_mon + 1);
	fileEntry.createTime[1] = (char)currentTime->tm_mday;
	fileEntry.createTime[2] = (char)currentTime->tm_hour;
	fileEntry.createTime[3] = (char)currentTime->tm_min;
	fileEntry.createTime[4] = (char)currentTime->tm_sec;
	
	strncpy(fileEntry.modifyTime,fileEntry.createTime, 5);
	fileEntry.modifyTimeYear = fileEntry.createTimeYear; 
	
	fwrite(&fileEntry.status, 1, 1, infile);
	fileEntryBigE.startBlock = htonl(fileEntry.startBlock);
	fileEntryBigE.numBlocks = htonl(fileEntry.numBlocks);
	fileEntryBigE.fileSize = htonl(fileEntry.fileSize);
	fileEntryBigE.createTimeYear = htons(fileEntry.createTimeYear);
	fileEntryBigE.modifyTimeYear = htons(fileEntry.modifyTimeYear);
	
	
	fwrite(&fileEntryBigE.startBlock, 1, 4, infile);
	fwrite(&fileEntryBigE.numBlocks, 1, 4, infile);
	fwrite(&fileEntryBigE.fileSize, 1, 4, infile);
	fwrite(&fileEntryBigE.createTimeYear, 1, 2, infile);
	fwrite(fileEntry.createTime, 1, 5, infile);
	fwrite(&fileEntryBigE.modifyTimeYear, 1, 2, infile);
	fwrite(fileEntry.modifyTime, 1, 5, infile);
	fwrite(fileEntry.fileName, 1, 31, infile);
	
	//printf("Write successful to block %d, terminating..\n", fileEntry.startBlock);
	fclose(infile);
	fclose(ToCopy);
	return 0;
}
