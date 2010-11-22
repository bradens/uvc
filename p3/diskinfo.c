#include "diskinfo.h"

int main(int argc, char**argv)
{
	if (argc < 1) {
		printf("Usage: ./diskinfo [.img file]\n");
		return 0;
	}
	
	FILE *infile = fopen(argv[1], "r");
	
	int *BlockSize, *FSCount, *StartPtr, *FATCount, *RootPtr, *RootCount;
	void *currentPtr = malloc(8);
	int currSizeSegment = 2;
	
	fread(currentPtr, 8, 1, infile);		/* read the superblock 	*/
	currentPtr = malloc(currSizeSegment);
	fread(currentPtr, currSizeSegment, 1, infile);
	BlockSize = (int*)currentPtr;
	
	currSizeSegment = 4;
	
	currentPtr = malloc(currSizeSegment);
	fread(currentPtr, currSizeSegment, 1, infile);
	
	FSCount = (int*)currentPtr;
	
	printf("Super block information:\n");
	printf("Block Size: %d\n", *BlockSize);
	printf("Block Count: %d\n", *FSCount);
	
	
	return 0;
}
