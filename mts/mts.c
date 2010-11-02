#include "mts.h"

int main (int argc, const char * argv[]) {
	eTopStack, eCurrent = eStack;
	
	/* init stack mutexes */
	pthread_mutex_init (&eStackMutex, NULL);
	void *status;
	
	ReadFile();
	for (LoadingCurrent = LoadingThreads; LoadingCurrent != NULL; LoadingCurrent++) {
		pthread_mutex_init(&LoadingCurrent->TMutex, NULL);
		pthread_cond_init(&LoadingCurrent->TState, NULL);
		
	
	
	pthread_create(&newTrain->tid, NULL, train, (void *)newTrain);
	push(newTrain);
	pthread_join(newTrain->tid, &status);
	printf("Popped Train %d\n.", pop()->TrainNumber);
    return 0;
}

int ReadFile() {
	/* load all the info from the input file */
	FILE* inFile = fopen("in.txt", "r");
	char* inLine = (char*)malloc(sizeof(char)*LINESIZE); 
	int ThreadCount = 0;
	LoadingCurrent = LoadingThreads;
	while (fgets(inLine, LINESIZE, inFile) != NULL) {
		LoadingCurrent->Direction = (char*)malloc(sizeof(char)*20);
		switch (inLine[0]) {
			case 'e':
				strcpy(LoadingCurrent->Direction, "EAST");
				LoadingCurrent->Priority = LOWPRI;
				break;
			case 'E':
				strcpy(LoadingCurrent->Direction, "EAST");
				LoadingCurrent->Priority = HIGHPRI;
				break;
			case 'w':
				strcpy(LoadingCurrent->Direction, "WEST");
				LoadingCurrent->Priority = LOWPRI;
				break;
			case 'W':
				strcpy(LoadingCurrent->Direction, "WEST");
				LoadingCurrent->Priority = HIGHPRI;
				break;
		}
		LoadingCurrent->TrainNumber = ThreadCount;
		inLine = (inLine + 2);									/* Skip over the Direction and : */
		LoadingCurrent->LoadingTime = atoi(strtok(inLine, ","));
		LoadingCurrent->CrossingTime = atoi(strtok(NULL, "\0"));
		LoadingCurrent++;
		ThreadCount++;
	}
	(LoadingCurrent + 1) = NULL;
	NumTrains = ThreadCount;
	return 0;
}

int push(TNode *t) {
	pthread_mutex_lock(&eStackMutex);
	eCurrent++;
	if (eCurrent == (eTopStack+STACKSIZE)) {
		printf("Stack Overflow.\n");
		exit(1);
	}
	eCurrent = t;
	pthread_mutex_unlock(&eStackMutex);
	return 0;
}

TNode *pop() {
	pthread_mutex_lock(&eStackMutex);
	if (eCurrent == eTopStack) {
		printf("Stack Underflow.\n");
		exit(1);
	}
	eCurrent--;
	pthread_mutex_unlock(&eStackMutex);
	return (eCurrent+1);
}

void *train (void *tnode) {
	TNode *Train = (TNode*)tnode;
	usleep(Train->LoadingTime * 1000000);
	printf("Train %d is ready to go %s\n", Train->TrainNumber, Train->Direction);
	pthread_exit(NULL);
}