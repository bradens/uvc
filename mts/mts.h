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

#define EHIGHPRISTATION 0
#define ELOWPRISTATION  1
#define WHIGHPRISTATION 2
#define WLOWPRISTATION  3
#define LINESIZE 50
#define STACKSIZE 100
#define HIGHPRI 1
#define LOWPRI 0

typedef struct thread_Node TNode;

struct thread_Node {
	pthread_mutex_t TMutex;
	pthread_cond_t TState;
	char* Direction;
	int Priority;
	int LoadingTime;
	int CrossingTime;
	pthread_t tid;
	int TrainNumber;
	TNode *next;
};

TNode *eHead, *eCurrent;			/* low pri east stack 	*/
TNode *EHead, *ECurrent;			/* high pri east stack 	*/
TNode *WHead, *WCurrent;			/* high pri west stack	*/
TNode *wHead, *wCurrent;			/* low pri west stack	*/
TNode LoadingThreads[STACKSIZE], *LoadingCurrent;
int NumTrains, TrainsFinished, TrackInUse, eStackCount, EStackCount, WStackCount, wStackCount;
char* LastDirection;
pthread_mutex_t eStackMutex, EStackMutex, WStackMutex, wStackMutex, TrackMutex;
pthread_cond_t TrackState;

TNode *top(TNode *head);
int push(TNode *t, int iStationNum);
TNode *pop(int iStationNum);
int ReadFile();
void *train(void *id);
