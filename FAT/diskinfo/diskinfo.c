/* 
 * Part I
 * Programming Assignment 3, Csc 360
 * Braden Simpson, V00685500
 */
#include "diskinfo.h"
int main(int argc, char**argv)
{
	if (argc < 2) {
		printf("Usage: ./diskinfo [.img file]\n");
		return 0;
	}
	FILE *infile = fopen(argv[1], "r");
	/* Respective parts of the superblock */
	int BlockSize, FSCount, StartPtr, FATCount, RootPtr, RootCount, block, FreeBlocksCount,
	ReservedBlocksCount, AllocatedBlocksCount;
	AllocatedBlocksCount = 0; ReservedBlocksCount = 0; FreeBlocksCount = 0;
	int currentSegmentSize = 2;							/* how large the current superblock segment is */
	void *currentPtr = malloc(currentSegmentSize);		/* the current segment of the superblock */
	fseek(infile, 8, SEEK_CUR);			/* Skip past the name */
	/* block determines which part of the superblock to examine */
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
				currentPtr = malloc(currentSegmentSize);
				fread(currentPtr, 1, currentSegmentSize, infile);
				memcpy(&FATCount, currentPtr, currentSegmentSize);
				FATCount = ntohl(FATCount);
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
	fseek(infile, 512 * StartPtr, SEEK_CUR);
	currentPtr = NULL;
	currentPtr = malloc(4);
	int hexVal, bytes;
	for (bytes = 0;;) {
		bytes += fread(currentPtr, 1, 4, infile);
		memcpy(&hexVal, currentPtr, 4);
		hexVal = ntohl(hexVal);
		switch (hexVal) {
			case 0x00000000:
				FreeBlocksCount++;
				break;
			case 0x00000001:
				ReservedBlocksCount++;
				break;
			default:
				AllocatedBlocksCount++;
				break;
		}
		free(currentPtr);
		currentPtr = NULL;
		if (bytes == 512*FATCount) break;
		currentPtr = malloc(4);
	}
	printf("Super block information:\nBlock Size: %d\nBlock Count: %d\nFAT starts: %d\nFAT blocks: %d\nRoot directory start: %d\nRoot directory blocks: %d\n",
	 BlockSize, FSCount, StartPtr, FATCount, RootPtr, RootCount);
	printf("\nFAT information:\nFree Blocks: %d\nReserved Blocks: %d\nAllocated Blocks: %d\n", FreeBlocksCount, ReservedBlocksCount, AllocatedBlocksCount);
	return 0;
}
