/*
 *  ssi.h
 *  SSI
 *
 *  Created by Braden on 12/09/10.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINESIZE 81

typedef struct node item;

char* readLine();
int ParseToken(char*);
item* FindRightMostSibling(item*);
item* FindNodeOnThisLevel(item*, char*);
item* FindPrevious(item*, char*);
int DoMkdir(char*);
int DoLSRecursive(item*); 
int DoLS(char*, int, item*);
int DoCd(char* options);
int DoRmdir();
int tokenizeLine(char*);
item* FindNode();
