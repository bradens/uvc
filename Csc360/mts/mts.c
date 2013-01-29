#include "mts.h"

int main (int argc, const char * argv[]) {
	/* Init stack pointers, capitilization shows priority and letter describes direction */
	EStack = (TStack *)malloc(sizeof(TStack));	
	eStack = (TStack *)malloc(sizeof(TStack));
	WStack = (TStack *)malloc(sizeof(TStack));
	wStack = (TStack *)malloc(sizeof(TStack));

	EStack->Head = NULL; eStack->Head = NULL; WStack->Head = NULL; wStack->Head = NULL;
	EStack->Count = 0; eStack->Count = 0; WStack->Count = 0; wStack->Count = 0;
	EStack->StackUse = 0; eStack->StackUse = 0; WStack->StackUse = 0; wStack->StackUse = 0;
	
	/* init parameters */
	inFileName = argv[1];
	NumTrains = atoi(argv[2]); TrainsFinished = 0;TrackInUse = 0;
	LastDirection = (char*)malloc(sizeof(char)*4);
	strcpy(LastDirection, "East");
	
	if (inFileName == NULL || NumTrains == 0) {
		printf("Usage : ./mts [inputfile] [number of trains]\n");
		return 1;
	}
	
	/* init stack mutexes */
	pthread_mutex_init (&eStack->Mutex, NULL);pthread_mutex_init (&wStack->Mutex, NULL);pthread_mutex_init (&WStack->Mutex, NULL);
	pthread_mutex_init (&EStack->Mutex, NULL);pthread_cond_init  (&EStack->Cv, NULL);pthread_cond_init  (&eStack->Cv, NULL);
	pthread_cond_init  (&WStack->Cv, NULL);pthread_cond_init  (&wStack->Cv, NULL);pthread_mutex_init (&TrackMutex, NULL);
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
		if (EStack->Count || WStack->Count) {		/* EStack or WStack is not NULL */
			if (EStack->Count && WStack->Count) {
				if (strcmp(LastDirection, "East") == 0) {		/* if the last direction was east, send west */
					pthread_mutex_lock(&TrackMutex);
					pthread_cond_signal(&WStack->Current->TState);
				}
				else {
					pthread_mutex_lock(&TrackMutex);
					pthread_cond_signal(&EStack->Current->TState);
				}
			}
			else if (EStack->Count) {		/* If EStack is not NULL */
				pthread_mutex_lock(&TrackMutex);
				pthread_cond_signal(&EStack->Current->TState);
			}
			else if (WStack->Count) {		/* If WStack is not NULL */
				pthread_mutex_lock(&TrackMutex);
				pthread_cond_signal(&WStack->Current->TState);
			}
		}
		else if (wStack->Count || eStack->Count)	{		/* eStack or wStack != NULL */
			if (eStack->Count && wStack->Count) {
				if (strcmp(LastDirection, "East") == 0) {	/* if the last direction was east, send west */
					pthread_mutex_lock(&TrackMutex);
					pthread_cond_signal(&wStack->Current->TState);
				}
				else {
					pthread_mutex_lock(&TrackMutex);
					pthread_cond_signal(&eStack->Current->TState);
				}
			}
			else if (eStack->Count) {		/* if the low pri east stack is not NULL */
				pthread_mutex_lock(&TrackMutex);
				pthread_cond_signal(&eStack->Current->TState);
			}
			else if (wStack->Count) {		/* if the low pri west stack is not NULL */
				pthread_mutex_lock(&TrackMutex);
				pthread_cond_signal(&wStack->Current->TState);
			}
		}
		pthread_mutex_unlock(&TrackMutex);
	}
	CleanUp();
	pthread_exit(NULL);
}

/*
 * Read the file and load the information into an array of Loading
 * Threads.
 */
int ReadFile() {
	/* open the input file */
	FILE* inFile = fopen(inFileName, "r");
	char* inLine = (char*)malloc(sizeof(char)*LINESIZE); 
	int ThreadCount = 0;
	LoadingCurrent = LoadingThreads;
	while (fgets(inLine, LINESIZE, inFile) != NULL) {
		LoadingCurrent->Direction = (char*)malloc(sizeof(char)*20);
		switch (inLine[0]) {
			case 'e':
				strcpy(LoadingCurrent->Direction, "East");
				LoadingCurrent->Priority = LOWPRI;
				break;
			case 'E':
				strcpy(LoadingCurrent->Direction, "East");
				LoadingCurrent->Priority = HIGHPRI;
				break;
			case 'w':
				strcpy(LoadingCurrent->Direction, "West");
				LoadingCurrent->Priority = LOWPRI;
				break;
			case 'W':
				strcpy(LoadingCurrent->Direction, "West");
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

/*
 * The function to be run in the train threads
 * this will sleep for the loading time, 
 * then push itself on the station stack.
 * Then wait for the scheduler to signal it 
 * to go.
 */
void *train (void *tnode) {
	TNode *Train = (TNode*)tnode;
	usleep(Train->LoadingTime * 100000);
	if (strcmp(Train->Direction, "East") == 0) {
		if (Train->Priority == HIGHPRI)
			push(Train, EStack);		
		else if (Train->Priority == LOWPRI)
			push(Train, eStack);
	}
	else if (strcmp(Train->Direction, "West") == 0) {
		if (Train->Priority == HIGHPRI)
			push(Train, WStack);		
		else if (Train->Priority == LOWPRI)
			push(Train, wStack);
	}
	printf("Train %2d is ready to go %4s\n", Train->TrainNumber, Train->Direction);
	pthread_mutex_lock(&Train->TMutex);
	pthread_cond_wait(&Train->TState, &Train->TMutex);
	TrackInUse = 1;
	if (strcmp(Train->Direction, "East") == 0) {
		if (Train->Priority == HIGHPRI)
			pop(EStack);		
		else if (Train->Priority == LOWPRI)
			pop(eStack);
	}
	else if (strcmp(Train->Direction, "West") == 0) {
		if (Train->Priority == HIGHPRI)
			pop(WStack);		
		else if (Train->Priority == LOWPRI)
			pop(wStack);
	}
	strcpy(LastDirection, Train->Direction);
	printf("Train %2d is ON the main track going %4s\n", Train->TrainNumber, Train->Direction);
	usleep(Train->CrossingTime * 100000);
	printf("Train %2d is OFF the main track after going %4s\n", Train->TrainNumber, Train->Direction);
	TrackInUse = 0;
	TrainsFinished++;
	pthread_cond_signal(&TrackState);
	pthread_mutex_unlock(&TrackMutex);	
	pthread_exit(NULL);
}

/* Train station implementation of stack push and orders the stack as it goes */
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

/* Train station implementation of stack pop */
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
		printf("Stack Underflow\n");
		pthread_mutex_unlock(&stack->Mutex);
		exit(1);
	}
	else {
		TNode *temp;
		if (stack->Count == 1) {
			stack->Count--;
			temp = stack->Head;
			stack->Head = NULL;
			stack->Current = temp;
			
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

/* CleanUp() function for graceful exit */
void CleanUp() 
{
	/* Since the nodes were freed when the threads exit, just need to free stacks */ 
	free(EStack);
	free(WStack);
	free(eStack);
	free(wStack);
	
	/* destroy all condition vars and mutexes */
	pthread_cond_destroy(&TrackState);
	pthread_mutex_destroy(&TrackMutex);
	free(LastDirection);
}