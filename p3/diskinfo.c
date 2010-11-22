#include "diskinfo.h"

int main(int argc, char**argv)
{
	if (argc < 1) {
		printf("Usage: ./diskinfo [.img file]\n");
		return 0;
	}
	int value;
	void *currentData = malloc(2);
	void *currentPtr = malloc(512000);
	FILE *infile = fopen(argv[1], "r");
	fread(currentPtr, 512000, 1, infile);	/* read the superblock */
	currentPtr += 8;
	memcpy(currentData, currentPtr, 2);
	value = (int*)currentData;
	printf("Super block information:\n");
	printf("%d\n", value);
	return 0;
}