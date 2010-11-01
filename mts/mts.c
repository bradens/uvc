#include "mts.h"

int main (int argc, const char * argv[]) {
	eTopStack, eCurrent = eStack;
	
	/* init stack mutexes */
	pthread_mutex_init (&eStackMutex, NULL);
	TNode *newTrain = (TNode*)malloc(sizeof(TNode));
	newTrain->Direction = (char*)malloc(sizeof(char)*20);
	void *status;
	/* TEST */
	/* first arg is Direction / Priority, second is load, 3rd is cross */
	if (strcmp(argv[1], "w") == 0) {
		strcpy(newTrain->Direction, "West");
		newTrain->Priority = LOWPRI;
	}
	newTrain->LoadingTime = atoi(argv[2]);
	newTrain->CrossingTime = atoi(argv[3]);
	pthread_create(&newTrain->tid, NULL, train, (void *)newTrain);
	push(newTrain);
	pthread_join(newTrain->tid, &status);
	printf("Popped Train %d\n.", pop()->TrainNumber);
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