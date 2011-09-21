/*
 *  rsi.h
 *  RSI
 *
 *  Created by Braden on 23/09/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
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
#include <time.h>

#define LINESIZE 100
#define BUFFSIZE 500
#define PROCESSNUM 100
#define EACCES 13
#define ENOENT 2
#define EIOERR 5

typedef struct struct_Process PNode;

int bg(char*[], char*);
int addPNode(char*, pid_t, PNode*, char*, int*);
int removePNode(pid_t);
int TokenizeLine(char*[], char*, char*);
int Exec(char*, char*[]);
int ParseCommand(char*, char*[], int, char*);
int cd(char*);
char* str_replace(char*, char*, char*, char*);
void ProcessChanged();
