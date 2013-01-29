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

/* Converts a input hex number and converts to decimal */
int htoi(int inputHex) {
	char s[7];
	sprintf(s, "%d", inputHex);
	return (int)strtol(s, NULL, 16);
}
