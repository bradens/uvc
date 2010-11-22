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

typedef struct thread_Node TNode;
typedef struct thread_Stack TStack;

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

struct thread_Stack {
	TNode *Head;
	TNode *Current;
	pthread_mutex_t Mutex;
	pthread_cond_t Cv;
	int Count;
	int StackUse;
};

TStack *EStack, *eStack, *WStack, *wStack;
TNode LoadingThreads[STACKSIZE], *LoadingCurrent;
int NumTrains, TrainsFinished, TrackInUse;
char* LastDirection;
const char *inFileName;
pthread_mutex_t TrackMutex;
pthread_cond_t TrackState;
int push(TNode *t, TStack *stack);
TNode *pop(TStack *stack);
int ReadFile();
void *train(void *id);
void CleanUp();
