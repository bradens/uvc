#include "mts.h"

int main (int argc, const char * argv[]) {
	/* Init stack pointers */
	EStack = (TStack *)malloc(sizeof(TStack));	
	eStack = (TStack *)malloc(sizeof(TStack));
	WStack = (TStack *)malloc(sizeof(TStack));
	wStack = (TStack *)malloc(sizeof(TStack));

	EStack->Head = NULL; eStack->Head = NULL; WStack->Head = NULL; wStack->Head = NULL;
	EStack->Count = 0; eStack->Count = 0; WStack->Count = 0; wStack->Count = 0;
	EStack->StackUse = 0; eStack->StackUse = 0; WStack->StackUse = 0; wStack->StackUse = 0;
	
	/* init parameters */
	NumTrains = atoi(argv[1]); TrainsFinished = 0;TrackInUse = 0;
	LastDirection = (char*)malloc(sizeof(char)*4);
	strcpy(LastDirection, "EAST");

	/* init stack mutexes */
	pthread_mutex_init (&eStack->Mutex, NULL);
	pthread_mutex_init (&wStack->Mutex, NULL);
	pthread_mutex_init (&WStack->Mutex, NULL);
	pthread_mutex_init (&EStack->Mutex, NULL);
	pthread_cond_init  (&EStack->Cv, NULL);
	pthread_cond_init  (&eStack->Cv, NULL);
	pthread_cond_init  (&WStack->Cv, NULL);
	pthread_cond_init  (&wStack->Cv, NULL);

	pthread_mutex_init (&TrackMutex, NULL);
	pthread_cond_init  (&TrackState, NULL);
		
	ReadFile();			/* Read the file, and setup the Loading array */
	int i = 0;
	for (LoadingCurrent = LoadingThreads; i < NumTrains; LoadingCurrent++) {
		pthread_mutex_init(&LoadingCurrent->TMutex, NULL);
		pthread_cond_init(&LoadingCurrent->TState, NULL);
		pthread_create(&LoadingCurrent->tid, NULL, train, (void *)LoadingCurrent);
		i++;	
	}

	/* Main scheduling loop, wait until all trains are finished. */
	while (TrainsFinished < NumTrains) {
		if (TrackInUse) {
			pthread_cond_wait(&TrackState, &TrackMutex);
			pthread_mutex_unlock(&TrackMutex);
			if (TrainsFinished == NumTrains)
				break;
		}
		if (TrainsFinished == -2) {
			printf("E:%d  W:%d  e:%d  w:%d  \n", EStack->Count, WStack->Count, eStack->Count, wStack->Count);
			sleep(1);
		}
		if (EStack->Count || WStack->Count) {
			if (EStack->Count && WStack->Count) {
				if (strcmp(LastDirection, "EAST") == 0) {
					pthread_mutex_lock(&TrackMutex);
					pthread_cond_signal(&WStack->Current->TState);
				}
				else {
					pthread_mutex_lock(&TrackMutex);
					pthread_cond_signal(&EStack->Current->TState);
				}
			}
			else if (EStack->Count) {
				pthread_mutex_lock(&TrackMutex);
				pthread_cond_signal(&EStack->Current->TState);
			}
			else if (WStack->Count) {
				pthread_mutex_lock(&TrackMutex);
				pthread_cond_signal(&WStack->Current->TState);
			}
		}
		else if (wStack->Count || eStack->Count)	{
			if (eStack->Count && wStack->Count) {
				if (strcmp(LastDirection, "EAST") == 0) {
					pthread_mutex_lock(&TrackMutex);
					pthread_cond_signal(&wStack->Current->TState);
				}
				else {
					pthread_mutex_lock(&TrackMutex);
					pthread_cond_signal(&eStack->Current->TState);
				}
			}
			else if (eStack->Count) {
				pthread_mutex_lock(&TrackMutex);
				pthread_cond_signal(&eStack->Current->TState);
			}
			else if (wStack->Count) {
				pthread_mutex_lock(&TrackMutex);
				pthread_cond_signal(&wStack->Current->TState);
			}
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
	return 0;
}

void *train (void *tnode) {
	TNode *Train = (TNode*)tnode;
	/*printf("Starting to load train %d.\n", Train->TrainNumber);
	*/usleep(Train->LoadingTime * 100000);
	if (strcmp(Train->Direction, "EAST") == 0) {
		if (Train->Priority == HIGHPRI)
			push(Train, EStack);		
		else if (Train->Priority == LOWPRI)
			push(Train, eStack);
	}
	else if (strcmp(Train->Direction, "WEST") == 0) {
		if (Train->Priority == HIGHPRI)
			push(Train, WStack);		
		else if (Train->Priority == LOWPRI)
			push(Train, wStack);
	}
	printf("Train %d is ready to go %s\n", Train->TrainNumber, Train->Direction);
	pthread_mutex_lock(&Train->TMutex);
	pthread_cond_wait(&Train->TState, &Train->TMutex);
	TrackInUse = 1;
	if (strcmp(Train->Direction, "EAST") == 0) {
		if (Train->Priority == HIGHPRI)
			pop(EStack);		
		else if (Train->Priority == LOWPRI)
			pop(eStack);
	}
	else if (strcmp(Train->Direction, "WEST") == 0) {
		if (Train->Priority == HIGHPRI)
			pop(WStack);		
		else if (Train->Priority == LOWPRI)
			pop(wStack);
	}
	LastDirection = Train->Direction;
	printf("Train %d is on the track going %s!\n", Train->TrainNumber, Train->Direction);
	usleep(Train->CrossingTime * 100000);
	printf("Train %d is done crossing\n", Train->TrainNumber);
	TrackInUse = 0;
	TrainsFinished++;
	pthread_cond_signal(&TrackState);
	pthread_mutex_unlock(&TrackMutex);	
	pthread_exit(NULL);
}

int push(TNode *t, TStack *stack) {
	TNode *checkTop;
	if (stack->StackUse) {
		pthread_cond_wait(&stack->Cv, &stack->Mutex);
		stack->StackUse = 1;
	}
	else {	
		pthread_mutex_lock(&stack->Mutex);
		stack->StackUse = 1;
	}
	if (stack->Head == NULL) {
		t->next = NULL;
		stack->Head = t;
		stack->Current = stack->Head;
		stack->Count++;
	}
	else {
		checkTop = stack->Current;
		if (checkTop->LoadingTime > t->LoadingTime) {
			TNode *prev = stack->Head;
			for (;prev->next->TrainNumber != stack->Current->TrainNumber;prev = prev->next) {}
			prev->next = t;
			t = checkTop;
		}
		else {
			stack->Current->next = t;
			t->next = NULL;
			stack->Current = t;
			stack->Count++;
		}
	}
	stack->StackUse = 0;
	pthread_cond_signal(&stack->Cv);
	pthread_mutex_unlock(&stack->Mutex);
	return 0;
}

TNode *pop(TStack *stack) {
	TNode *prev;
	if (stack->StackUse) {
		pthread_cond_wait(&stack->Cv, &stack->Mutex);
		stack->StackUse = 1;
	}
	else {	
		pthread_mutex_lock(&stack->Mutex);
		stack->StackUse = 1;
	}
	if (stack->Count == 0) {
		printf("Stack Undeflow\n");
		pthread_mutex_unlock(&stack->Mutex);
		exit(1);
	}
	else {
		TNode *temp;
		if (stack->Count == 1) {
			stack->Count--;
			temp = stack->Head;
			stack->Head = NULL;
			stack->Current = NULL;
		}
		else {
			for (prev = stack->Head;prev->next != NULL;prev = prev->next) {stack->Current = prev;}
			stack->Count--;
			temp = prev;
			stack->Current->next = NULL;
		}
		stack->StackUse = 0;
		pthread_cond_signal(&stack->Cv);
		pthread_mutex_unlock(&stack->Mutex);
		return (temp);
	}
	return 0;
}
