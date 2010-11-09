#include "mts.h"

int main (int argc, const char * argv[]) {
	/* Init stack pointers */
	eHead = NULL; wHead = NULL; WHead = NULL; EHead = NULL;
	eStackCount = 0; EStackCount = 0; wStackCount = 0;
	WStackCount = 0; EStackUse = 0; WStackUse = 0; wStackUse = 0;
	eStackUse = 0;
	
	/* init stack mutexes */
	pthread_mutex_init (&eStackMutex, NULL);
	pthread_mutex_init (&wStackMutex, NULL);
	pthread_mutex_init (&WStackMutex, NULL);
	pthread_mutex_init (&EStackMutex, NULL);
	pthread_mutex_init (&TrackMutex, NULL);
	pthread_cond_init  (&TrackState, NULL);
	pthread_cond_init  (&EStackCv, NULL);
	pthread_cond_init  (&eStackCv, NULL);
	pthread_cond_init  (&WStackCv, NULL);
	pthread_cond_init  (&wStackCv, NULL);
	
	NumTrains = atoi(argv[1]); TrainsFinished = 0;TrackInUse = 0;
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
		if (TrackInUse) {
			pthread_cond_wait(&TrackState, &TrackMutex);
			pthread_mutex_unlock(&TrackMutex);
			if (TrainsFinished == NumTrains)
				break;
		}
		if (TrainsFinished == -2) {
			printf("E:%d  W:%d  e:%d  w:%d  \n", EStackCount, WStackCount, eStackCount, wStackCount);
			sleep(1);
		}
		if (EStackCount || WStackCount) {
			if (EStackCount && WStackCount) {
				if (strcmp(LastDirection, "EAST") == 0) {
					pthread_mutex_lock(&TrackMutex);
					pthread_cond_signal(&(top(WHead)->TState));
				}
				else {
					pthread_mutex_lock(&TrackMutex);
					pthread_cond_signal(&(top(EHead)->TState));
				}
			}
			else if (EStackCount) {
				pthread_mutex_lock(&TrackMutex);
				pthread_cond_signal(&(top(EHead)->TState));
			}
			else if (WStackCount) {
				pthread_mutex_lock(&TrackMutex);
				pthread_cond_signal(&(top(WHead)->TState));
			}
		}
		else if (wStackCount || eStackCount)	{
			if (eStackCount && wStackCount) {
				if (strcmp(LastDirection, "EAST") == 0) {
					pthread_mutex_lock(&TrackMutex);
					pthread_cond_signal(&(top(wHead)->TState));
				}
				else {
					pthread_mutex_lock(&TrackMutex);
					pthread_cond_signal(&(top(eHead)->TState));
				}
			}
			else if (eStackCount) {
				pthread_mutex_lock(&TrackMutex);
				pthread_cond_signal(&(top(eHead)->TState));
			}
			else if (wStackCount) {
				pthread_mutex_lock(&TrackMutex);
				pthread_cond_signal(&(top(wHead)->TState));
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
	TrainsFinished++;
	pthread_cond_signal(&TrackState);
	pthread_mutex_unlock(&TrackMutex);	
	pthread_exit(NULL);
}

int push(TNode *t, int iStationNum) {
	TNode *checkTop;
	switch (iStationNum) {
		case EHIGHPRISTATION:
			if (EStackUse) {
				pthread_cond_wait(&EStackCv, &EStackMutex);
				EStackUse = 1;
			}
			else {	
				pthread_mutex_lock(&EStackMutex);
				EStackUse = 1;
			}
			if (EHead == NULL) {
				t->next = NULL;
				EHead = t;
				ECurrent = EHead;
				EStackCount++;
			}
			else {
				checkTop = ECurrent;
				if (checkTop->LoadingTime > t->LoadingTime) {
					TNode *prev = EHead;
					for (;prev->next->TrainNumber != ECurrent->TrainNumber;prev = prev->next) {}
					prev->next = t;
					t = checkTop;
				}
				else {
					ECurrent->next = t;
					t->next = NULL;
					ECurrent = t;
					EStackCount++;
				}
			}
			EStackUse = 0;
			pthread_cond_signal(&EStackCv);
			pthread_mutex_unlock(&EStackMutex);
			break;
		case WHIGHPRISTATION:
			if (WStackUse) {
				pthread_cond_wait(&WStackCv, &WStackMutex);
				WStackUse = 1;
			}
			else {	
				pthread_mutex_lock(&WStackMutex);
				WStackUse = 1;
			}
			if (WHead == NULL) {
				t->next = NULL;
				WHead = t;
				WCurrent = WHead;
				WStackCount++;
			}
			else {
				checkTop = WCurrent;
				if (checkTop->LoadingTime > t->LoadingTime) {
					TNode *prev = WHead;
					for (;prev->next->TrainNumber != WCurrent->TrainNumber;prev = prev->next) {}
					prev->next = t;
					t = checkTop;
				}
				else {
					WCurrent->next = t;
					t->next = NULL;
					WCurrent = t;
					WStackCount++;
				}
			}
			WStackUse = 0;
			pthread_cond_signal(&WStackCv);
			pthread_mutex_unlock(&WStackMutex);
			break;
		case WLOWPRISTATION:
			if (wStackUse) {
				pthread_cond_wait(&wStackCv, &wStackMutex);
				wStackUse = 1;
			}
			else {	
				pthread_mutex_lock(&wStackMutex);
				wStackUse = 1;
			}
			if (wHead == NULL) {
				t->next = NULL;
				wHead = t;
				wCurrent = wHead;
				wStackCount++;
			}
			else {
				checkTop = wCurrent;
				if (checkTop->LoadingTime > t->LoadingTime) {
					TNode *prev = wHead;
					for (;prev->next->TrainNumber != wCurrent->TrainNumber;prev = prev->next) {}
					prev->next = t;
					t = checkTop;
				}
				else {
					wCurrent->next = t;
					t->next = NULL;
					wCurrent = t;
					wStackCount++;
				}
			}
			wStackUse = 0;
			pthread_cond_signal(&wStackCv);
			pthread_mutex_unlock(&wStackMutex);
			break;
		case ELOWPRISTATION:
			if (eStackUse) {
				pthread_cond_wait(&eStackCv, &eStackMutex);
				eStackUse = 1;
			}
			else {	
				pthread_mutex_lock(&eStackMutex);
				eStackUse = 1;
			}
			if (eHead == NULL) {
				t->next = NULL;
				eHead = t;
				eCurrent = eHead;
				eStackCount++;
			}
			else {
				checkTop = eCurrent;
				if (checkTop->LoadingTime > t->LoadingTime) {
					TNode *prev = eHead;
					for (;prev->next->TrainNumber != eCurrent->TrainNumber;prev = prev->next) {}
					prev->next = t;
					t = checkTop;
				}
				else {
					eCurrent->next = t;
					t->next = NULL;
					eCurrent = t;
					eStackCount++;
				}
			}
			eStackUse = 0;
			pthread_cond_signal(&eStackCv);
			pthread_mutex_unlock(&eStackMutex);
			break;
	}
	return 0;
}

TNode *pop(int iStationNum) {
	TNode *prev;
	switch (iStationNum) {
		case EHIGHPRISTATION:
			if (EStackUse) {
				pthread_cond_wait(&EStackCv, &EStackMutex);
				EStackUse = 1;
			}
			else {	
				pthread_mutex_lock(&EStackMutex);
				EStackUse = 1;
			}
			if (EStackCount == 0) {
				printf("Stack Undeflow\n");
				pthread_mutex_unlock(&EStackMutex);
				exit(1);
			}
			else {
				TNode *temp;
				if (EStackCount == 1) {
					EStackCount--;
					temp = EHead;
					EHead = NULL;
					ECurrent = NULL;
				}
				else {
					for (prev = EHead;prev->next != NULL;prev = prev->next) {ECurrent = prev;}
					EStackCount--;
					temp = prev;
					ECurrent->next = NULL;
				}
				EStackUse = 0;
				pthread_cond_signal(&EStackCv);
				pthread_mutex_unlock(&EStackMutex);
				return (temp);
			}
			break;
		case WHIGHPRISTATION:
			if (WStackUse) {
				pthread_cond_wait(&WStackCv, &WStackMutex);
				WStackUse = 1;
			}
			else {	
				pthread_mutex_lock(&WStackMutex);
				WStackUse = 1;
			}
			if (WStackCount == 0) {
				printf("Stack Underflow\n");
				pthread_mutex_unlock(&WStackMutex);
				exit(1);
			}
			else {
				TNode *temp;
				if (WStackCount == 1) {
					WStackCount--;
					temp = WHead;
					WHead = NULL;
					WCurrent = NULL;
				}
				else {
					for (prev = WHead;prev->next != NULL;prev = prev->next) {WCurrent = prev;}
					WStackCount--;
					temp = prev;
					WCurrent->next = NULL;
				}
				WStackUse = 0;
				pthread_cond_signal(&WStackCv);
				pthread_mutex_unlock(&WStackMutex);
				return (temp);
			}
			break;
		case WLOWPRISTATION:
			if (wStackUse) {
				pthread_cond_wait(&wStackCv, &wStackMutex);
				wStackUse = 1;
			}
			else {	
				pthread_mutex_lock(&wStackMutex);
				wStackUse = 1;
			}
			if (wStackCount == 0) {
				printf("Stack Undeflow\n");
				pthread_mutex_unlock(&wStackMutex);
				exit(1);
			}
			else {
				TNode *temp;
				if (wStackCount == 1) {
					wStackCount--;
					temp = wHead;
					wHead = NULL;
					wCurrent = NULL;
				}
				else {
					for (prev = wHead;prev->next != NULL;prev = prev->next) {wCurrent = prev;}
					wStackCount--;
					temp = prev;
					wCurrent->next = NULL;
				}
				wStackUse = 0;
				pthread_cond_signal(&wStackCv);
				pthread_mutex_unlock(&wStackMutex);
				return (temp);
			}
			break;
		case ELOWPRISTATION:
			if (eStackUse) {
				pthread_cond_wait(&eStackCv, &eStackMutex);
				eStackUse = 1;
			}
			else {	
				pthread_mutex_lock(&eStackMutex);
				eStackUse = 1;
			}
			if (eStackCount == 0) {
				printf("Stack Undeflow\n");
				pthread_mutex_unlock(&eStackMutex);
				exit(1);
			}
			else {
				TNode *temp;
				if (eStackCount == 1) {
					eStackCount--;
					temp = eHead;
					eHead = NULL;
					eCurrent = NULL;
				}
				else {
					for (prev = eHead;prev->next != NULL;prev = prev->next) {eCurrent = prev;}
					eStackCount--;
					temp = prev;
					eCurrent->next = NULL;
				}
				eStackUse = 0;
				pthread_cond_signal(&eStackCv);
				pthread_mutex_unlock(&eStackMutex);
				return (temp);
			}
			break;
	}
	return 0;
}
	
TNode *top(TNode *head) {
	for(;head->next != NULL;head = head->next){};
	return head;
}
