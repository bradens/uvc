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
	AllocatedBlocksCount = 0;ReservedBlocksCount = 0;FreeBlocksCount = 0;
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
	fseek(infile, RootPtr*16, SEEK_SET);
	DEntry root;
	fread(&root.status, 1, 1, infile);
	printf("Bit 0 of status byte: %d\n", CHECK_BIT(root.status, 0));
	/*
	int FatTable[FATCount];
	printf("%d\n", FATCount);
	int a = fread(FatTable, sizeof(int), FATCount, infile);
	printf("bytes read: %d\n", a);
	for (block = 0;block < FATCount;block++) {
		switch(ntohl(FatTable[block])) {
			case 0:
				FreeBlocksCount++;
				break;
			case 0x00000001:
				ReservedBlocksCount++;
				break;
			default:
				AllocatedBlocksCount++;
				break;
		}
	}*/
		
	printf("Super block information:\n");
	printf("Block Size: %d\n", BlockSize);
	printf("Block Count: %d\n", FSCount);
	printf("FAT starts: %d\n", StartPtr);
	printf("FAT blocks: %d\n", FATCount);
	printf("Root directory start: %d\n", RootPtr);
	printf("Root directory blocks: %d\n", RootCount);
	printf("\nFAT information:\n");
	printf("Free Blocks: %d\n", FreeBlocksCount);
	printf("Reserved Blocks: %d\n", ReservedBlocksCount);
	printf("Allocated Blocks: %d\n", AllocatedBlocksCount);
	return 0;
}
