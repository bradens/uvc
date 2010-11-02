/*
 *  mts.h
 *  mts
 *
 *  Created by Braden on 01/11/10.
 *
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 50
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

TNode *eTopStack, eStack[STACKSIZE], *eCurrent;			/* low pri east stack 	*/
TNode *ETopStack, EStack[STACKSIZE], *ECurrent;			/* high pri east stack 	*/
TNode *WTopStack, WStack[STACKSIZE], *WCurrent;			/* high pri west stack	*/
TNode *wTopStack, wStack[STACKSIZE], *wCurrent;			/* low pri west stack	*/
TNode LoadingThreads[STACKSIZE], *LoadingCurrent;
int NumTrains, TrainsFinished, TrackInUse;
char* LastDirection;
pthread_mutex_t eStackMutex, EStackMutex, WStackMutex, wStackMutex, TrackMutex;
pthread_cond_t TrackState;
int push(TNode *t, TNode *StackTop, TNode *StackCurrent, pthread_mutex_t *stackMutex);
int ReadFile();
TNode *pop(TNode *StackCurrent, TNode *StackTop, pthread_mutex_t *stackMutex);
void *train(void *id);
