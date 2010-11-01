/*
 *  mts.h
 *  mts
 *
 *  Created by Braden on 01/11/10.
 *
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define STACKSIZE 100
#define HIGHPRI 1
#define LOWPRI 0

typedef struct thread_Node {
	pthread_mutex_t TMutex;
	pthread_cond_t TState;
	char* Direction;
	int Priority;
	int LoadingTime;
	int CrossingTime;
	pthread_t tid;
	int TrainNumber;
}TNode;

TNode *eTopStack, eStack[STACKSIZE], *eCurrent;
pthread_mutex_t eStackMutex;
int push(TNode *t);
TNode *pop();
void *train(void *id);
