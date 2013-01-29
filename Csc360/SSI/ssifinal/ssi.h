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
/* LINESIZE is 400 because each directory can be 80 characters long
 * and I allow for 45 directories to be in the commands (i.e. cd test/test2/test3/test4/ 
 * etc)
 */
#define LINESIZE 400

typedef struct node item;

char* readLine();
int ParseToken(char*);
int DoMkdir(char*);
int DoLSRecursive(item*); 
int DoLS(char*, int, item*);
int DoCd(char* options);
int DoRmdir();
int tokenizeLine(char*);
int dispose();
int FreeAllNodes();
item* FindRightMostSibling(item*);
item* FindNodeOnThisLevel(item*, char*);
item* FindPrevious(item*, char*);
item* FindNode(char*);
