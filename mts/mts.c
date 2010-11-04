#include "mts.h"

int main (int argc, const char * argv[]) {
	/* Init stack pointers */
	eTopStack = eStack; eCurrent = eStack; ETopStack = EStack; ECurrent = EStack; wTopStack = wStack;
	wCurrent = wStack; WTopStack = WStack; WCurrent = WStack; eStackCount = 1; EStackCount = 0; wStackCount = 0;
	WStackCount = 0; 
	
	/* init stack mutexes */
	pthread_mutex_init (&eStackMutex, NULL);
	pthread_mutex_init (&wStackMutex, NULL);
	pthread_mutex_init (&WStackMutex, NULL);
	pthread_mutex_init (&EStackMutex, NULL);
	pthread_mutex_init (&TrackMutex, NULL);
	pthread_cond_init  (&TrackState, NULL);
	
	NumTrains = 0; TrainsFinished = 0;TrackInUse = 0;
	LastDirection = (char*)malloc(sizeof(char)*4);
	strcpy(LastDirection, "EAST");
	ReadFile();
	int i = 0;
	for (LoadingCurrent = LoadingThreads; i < NumTrains; LoadingCurrent++) {
		pthread_mutex_init(&LoadingCurrent->TMutex, NULL);
		pthread_cond_init(&LoadingCurrent->TState, NULL);
		pthread_create(&LoadingCurrent->tid, NULL, train, (void *)LoadingCurrent);
		i++;	
	}
	while (TrainsFinished < NumTrains) {
		if (TrackInUse)
			pthread_cond_wait(&TrackState, &TrackMutex);
		if (EStackCount || WStackCount) {
			if (EStackCount && WStackCount) {
				if (strcmp(LastDirection, "EAST") == 0)
					pthread_cond_signal(&(WCurrent->TState));
				else
					pthread_cond_signal(&(ECurrent->TState));
			}
			else if (EStackCount)
				pthread_cond_signal(&(ECurrent->TState));
			else if (WStackCount)
				pthread_cond_signal(&(WCurrent->TState));
		}
		else if (wStackCount || eStackCount)	{
			if (eStackCount && wStackCount) {
				if (strcmp(LastDirection, "EAST") == 0)
					pthread_cond_signal(&(wCurrent->TState));
				else
					pthread_cond_signal(&(eCurrent->TState));
			}
			else if (eStackCount)
				pthread_cond_signal(&(eCurrent->TState));
			else if (wStackCount)
				pthread_cond_signal(&(wCurrent->TState));
		}
		pthread_mutex_unlock(&TrackMutex);
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

int push(TNode *t, int iStationNum) {
	TNode *checkTop;
	switch (iStationNum) {
		case EHIGHPRISTATION:
			pthread_mutex_lock(&EStackMutex);
			checkTop = ECurrent;
			if (checkTop->LoadingTime >= t->LoadingTime) {					/* If the top item on the stack has a larger loading 	*/
			ECurrent = t;												/* time than the node being added, swap them			*/
			t = checkTop;
			}
			ECurrent++;
			EStackCount++;
			if (ECurrent == (ETopStack+STACKSIZE)) {
				printf("Stack Overflow.\n");
				exit(1);
			}
			ECurrent = t;
			pthread_mutex_unlock(&EStackMutex);
			break;
		case WHIGHPRISTATION:
			pthread_mutex_lock(&WStackMutex);
			checkTop = WCurrent;
			if (checkTop->LoadingTime >= t->LoadingTime) {					/* If the top item on the stack has a larger loading 	*/
			WCurrent = t;													/* time than the node being added, swap them			*/
			t = checkTop;
			}
			WCurrent++;
			WStackCount++;
			if (WCurrent == (WTopStack+STACKSIZE)) {
				printf("Stack Overflow.\n");
				exit(1);
			}
			WCurrent = t;
			pthread_mutex_unlock(&WStackMutex);
		case WLOWPRISTATION:
			pthread_mutex_lock(&wStackMutex);
			checkTop = wCurrent;
			if (checkTop->LoadingTime >= t->LoadingTime) {					/* If the top item on the stack has a larger loading 	*/
			wCurrent = t;													/* time than the node being added, swap them			*/
			t = checkTop;
			}
			wCurrent++;
			wStackCount++;
			if (wCurrent == (wTopStack+STACKSIZE)) {
				printf("Stack Overflow.\n");
				exit(1);
			}
			wCurrent = t;
			pthread_mutex_unlock(&wStackMutex);
			break;
		case ELOWPRISTATION:
			pthread_mutex_lock(&eStackMutex);
			checkTop = eCurrent;
			if (checkTop->LoadingTime >= t->LoadingTime) {					/* If the top item on the stack has a larger loading 	*/
			eCurrent = t;													/* time than the node being added, swap them			*/
			t = checkTop;
			}
			eCurrent++;
			eStackCount++;
			if (eCurrent == (eTopStack+STACKSIZE)) {
				printf("Stack Overflow.\n");
				exit(1);
			}
			eCurrent = t;
			pthread_mutex_unlock(&eStackMutex);
			break;
	}
	return 0;
}

TNode *pop(iStationNum) {
	switch (iStationNum) {
		case EHIGHPRISTATION:
			pthread_mutex_lock(&EStackMutex);
			if (ECurrent == ETopStack) {
				printf("Stack Underflow.\n");
				exit(1);
			}
			ECurrent = ECurrent - 1;
			EStackCount--;
			pthread_mutex_unlock(&EStackMutex);
			return (ECurrent+1);
			break;
		case ELOWPRISTATION:
			pthread_mutex_lock(&eStackMutex);
			if (eCurrent == eTopStack) {
				printf("Stack Underflow.\n");
				exit(1);
			}
			eCurrent = eCurrent - 1;
			eStackCount--;
			pthread_mutex_unlock(&eStackMutex);
			return (eCurrent+1);
		case WHIGHPRISTATION:
			pthread_mutex_lock(&WStackMutex);
			if (WCurrent == WTopStack) {
				printf("Stack Underflow.\n");
				exit(1);
			}
			WCurrent = WCurrent - 1;
			WStackCount--;
			pthread_mutex_unlock(&WStackMutex);
			return (WCurrent+1);
		case WLOWPRISTATION:
			pthread_mutex_lock(&wStackMutex);
			if (wCurrent == wTopStack) {
				printf("Stack Underflow.\n");
				exit(1);
			}
			wCurrent = wCurrent - 1;
			wStackCount--;
			pthread_mutex_unlock(&wStackMutex);
			return (wCurrent+1);
	}
	return 0;
}

void *train (void *tnode) {
	TNode *Train = (TNode*)tnode;
	printf("Starting to load train %d.\n", Train->TrainNumber);
	usleep(Train->LoadingTime * 100000);
	if (strcmp(Train->Direction, "EAST") == 0) {
		if (Train->Priority == HIGHPRI)
			push(Train, EHIGHPRISTATION);		
		else if (Train->Priority == LOWPRI)
			push(Train, ELOWPRISTATION);
	}
	else if (strcmp(Train->Direction, "WEST") == 0) {
		if (Train->Priority == HIGHPRI)
			push(Train, WHIGHPRISTATION);		
		else if (Train->Priority == LOWPRI)
			push(Train, WLOWPRISTATION);
	}
	printf("Train %d is ready to go %s\n", Train->TrainNumber, Train->Direction);
	pthread_mutex_lock(&Train->TMutex);
	pthread_cond_wait(&Train->TState, &Train->TMutex);
	TrackInUse = 1;
	if (strcmp(Train->Direction, "EAST") == 0) {
		if (Train->Priority == HIGHPRI)
			pop(EHIGHPRISTATION);		
		else if (Train->Priority == LOWPRI)
			pop(ELOWPRISTATION);
	}
	else if (strcmp(Train->Direction, "WEST") == 0) {
		if (Train->Priority == HIGHPRI)
			pop(WHIGHPRISTATION);		
		else if (Train->Priority == LOWPRI)
			pop(WLOWPRISTATION);
	}
	free(LastDirection);
	LastDirection = Train->Direction;
	printf("Train %d is on the track going %s!\n", Train->TrainNumber, Train->Direction);
	usleep(Train->CrossingTime * 100000);
	printf("Train %d is done crossing\n", Train->TrainNumber);
	TrackInUse = 0;
	pthread_cond_signal(&TrackState);
	pthread_mutex_unlock(&TrackMutex);	
	TrainsFinished++;
	pthread_exit(NULL);
}
