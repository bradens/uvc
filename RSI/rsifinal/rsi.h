/*
 *  rsi.h
 *  RSI
 *
 *  Created by Braden on 23/09/10.
 *  programming assignment 1, csc 360 AO1
 *  V00685500
 *
 */

#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#define LINESIZE 100
#define BUFFSIZE 500
#define PROCESSNUM 100
#define EACCES 13
#define ENOENT 2
#define EIOERR 5

typedef struct struct_Process PNode;

int bg(char*[]);
int bglist();
int bgkill(pid_t PID);
int bgstop(pid_t PID);
int bgstart(pid_t PID);
int addPNode(char*, pid_t, PNode*, char*, int*);
int removePNode(pid_t);
int TokenizeLine(char*[], char*, char*);
int Exec(char*, char*[]);
int ParseCommand(char*, char*[], int, char*);
int cd(char*);
int Destroy();
char* str_replace(char*, char*, char*, char*);
void ProcessChanged();
