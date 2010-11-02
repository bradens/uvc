#include "mts.h"

/*
 * PROBLEM, POINTER ISNT UPDATING VALUES IN PUSH/POP
 */

int main (int argc, const char * argv[]) {
	/* Init stack pointers */
	eTopStack = eStack; eCurrent = eStack; ETopStack = EStack; ECurrent = EStack; wTopStack = wStack;
	wCurrent = wStack; WTopStack = WStack; WCurrent = WStack;
	
	/* init stack mutexes */
	pthread_mutex_init (&eStackMutex, NULL);
	pthread_mutex_init (&wStackMutex, NULL);
	pthread_mutex_init (&WStackMutex, NULL);
	pthread_mutex_init (&EStackMutex, NULL);
	pthread_mutex_init (&TrackMutex, NULL);
	pthread_cond_init  (&TrackState, NULL);
	
	NumTrains = 0; TrainsFinished = 0;TrackInUse = 0;
	
	ReadFile();
	int i = 0;
	for (LoadingCurrent = LoadingThreads; i < NumTrains; LoadingCurrent++) {
		pthread_mutex_init(&LoadingCurrent->TMutex, NULL);
		pthread_cond_init(&LoadingCurrent->TState, NULL);
		pthread_create(&LoadingCurrent->tid, NULL, train, (void *)LoadingCurrent);
		i++;	
	}
	sleep(5);
	/* Write out scheduling algorithm */
	while (TrainsFinished < NumTrains) {
		printf("%d\n", wCurrent->TrainNumber);
		pthread_mutex_lock(&TrackMutex);
		if (ECurrent != ETopStack || WCurrent != WTopStack) {
			if (ECurrent != ETopStack && WCurrent != WTopStack) {
				if (strcmp(LastDirection, "EAST") == 0)
					pthread_cond_signal(&(pop(ECurrent, ETopStack, &EStackMutex)->TState));
				else
					pthread_cond_signal(&(pop(WCurrent, WTopStack, &WStackMutex)->TState));
			}
			else if (ECurrent != ETopStack)
				pthread_cond_signal(&(pop(ECurrent, ETopStack, &EStackMutex)->TState));
			else if (WCurrent != WTopStack)
				pthread_cond_signal(&(pop(WCurrent, WTopStack, &WStackMutex)->TState));
		}
		else if (wCurrent != wTopStack || eCurrent != eTopStack)	{
			if (eCurrent != eTopStack && wCurrent != wTopStack) {
				if (strcmp(LastDirection, "EAST") == 0)
					pthread_cond_signal(&(pop(eCurrent, eTopStack, &eStackMutex)->TState));
				else
					pthread_cond_signal(&(pop(wCurrent, wTopStack, &wStackMutex)->TState));
			}
			else if (eCurrent != eTopStack)
				pthread_cond_signal(&(pop(eCurrent, eTopStack, &eStackMutex)->TState));
			else if (wCurrent != wTopStack)
				pthread_cond_signal(&(pop(wCurrent, wTopStack, &wStackMutex)->TState));
		}
		else {
			if (TrackInUse == 0)
				pthread_mutex_unlock(&TrackMutex);
		}
	}
    	pthread_exit(NULL);
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
	NumTrains = ThreadCount;
	return 0;
}

int push(TNode *t, TNode *StackTop, TNode *StackCurrent, pthread_mutex_t *stackMutex) {
	pthread_mutex_lock(stackMutex);
	TNode *checkTop = StackCurrent;
	if (checkTop->LoadingTime >= t->LoadingTime) {					/* If the top item on the stack has a larger loading 	*/
											/* time than the node being added, swap them		*/
		StackCurrent = t;
		t = checkTop;
	}
	StackCurrent++;
	if (StackCurrent == (StackTop+STACKSIZE)) {
		printf("Stack Overflow.\n");
		exit(1);
	}
	StackCurrent = t;
	pthread_mutex_unlock(stackMutex);
	return 0;
}

TNode *pop(TNode *StackCurrent, TNode *StackTop, pthread_mutex_t *stackMutex) {
	pthread_mutex_lock(stackMutex);
	if (StackCurrent == StackTop) {
		printf("Stack Underflow.\n");
		exit(1);
	}
	StackCurrent = StackCurrent - 1;
	pthread_mutex_unlock(stackMutex);
	return (StackCurrent+1);
}

void *train (void *tnode) {
	TNode *Train = (TNode*)tnode;
	printf("Starting to load train %d.\n", Train->TrainNumber);
	usleep(Train->LoadingTime * 100000);
	if (strcmp(Train->Direction, "EAST") == 0) {
		if (Train->Priority == HIGHPRI)
			push(Train, ETopStack, ECurrent, &EStackMutex);		
		else if (Train->Priority == LOWPRI)
			push(Train, eTopStack, eCurrent, &eStackMutex);
	}
	else if (strcmp(Train->Direction, "WEST") == 0) {
		if (Train->Priority == HIGHPRI)
			push(Train, WTopStack, WCurrent, &WStackMutex);		
		else if (Train->Priority == LOWPRI)
			push(Train, wTopStack, wCurrent, &wStackMutex);
	}
	printf("Train %d is ready to go %s\n", Train->TrainNumber, Train->Direction);
	pthread_mutex_lock(&Train->TMutex);
	pthread_cond_wait(&Train->TState, &Train->TMutex);
	TrackInUse = 1;
	if (strcmp(Train->Direction, "EAST") == 0) {
		if (Train->Priority == HIGHPRI)
			pop(ECurrent, ETopStack, &EStackMutex);		
		else if (Train->Priority == LOWPRI)
			pop(eCurrent, eTopStack, &eStackMutex);
	}
	else if (strcmp(Train->Direction, "WEST") == 0) {
		if (Train->Priority == HIGHPRI)
			pop(WCurrent, WTopStack, &WStackMutex);		
		else if (Train->Priority == LOWPRI)
			pop(wCurrent, wTopStack, &wStackMutex);
	}
	LastDirection = Train->Direction;
	printf("Train %d is on the track going %s!\n", Train->TrainNumber, Train->Direction);
	usleep(Train->CrossingTime * 100000);
	printf("Train %d is done crossing\n", Train->TrainNumber);
	TrackInUse = 0;
	pthread_cond_signal(&TrackState);
	pthread_mutex_unlock(&TrackMutex);	
	pthread_exit(NULL);
}
