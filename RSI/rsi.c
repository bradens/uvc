/*
 * rsi.c
 * Braden simpson
 * Programming assignment 1, csc 360
 * V00685500
 */

#include "rsi.h"

/* define a structure to hold all the process information */
struct struct_Process {
	pid_t	PID;		/* PID of the process 			*/
	int*	iStatus;	/* Integer status of the process 	*/
	int	iNumber;	/* Process number from time created 	*/
	char*	Status;		/* Char* Status of the process		*/
	char*	Command;	/* Command of the process		*/
	PNode*	Next;		/* Next process node in the list	*/
};

PNode*	m_Head = NULL;		/* Pointer to head of the Process List	*/
char*	m_InputLine;		/* Char pointer to the input line 	*/

int main (int argc, const char * argv[]) {
	char *Prompt, *Command;
	PNode* Current;							
	char **options;		/* The tokens in the lines separated by spaces 	*/
	char *InputLineCopy;
	int ErrCheck = 0;	/* integer for checking return values		*/
	int iArgsCount = 0;	/* integer for counting the number of arguments */

	
        signal(SIGCHLD, ProcessChanged);	/* set up the signal handler 			*/
	
	for(;;) {				/* waiting for user input loop 			*/
		options = (char**)malloc(sizeof(char*)*LINESIZE);
		Prompt = getcwd(NULL, 0);					/* Get the current working directory		*/
		strcat(Prompt, " $ ");						/* concatenate the "$" on the prompt 		*/
		
		/* check if processes are finished */
		for (Current = m_Head;Current != NULL;Current = Current->Next) {
			if (strcmp(Current->Status, "finished") == 0) {
				/* found a process that is ready to be discarded */
				printf("[%d] %d: %s %s\n", Current->iNumber, Current->PID, Current->Status, Current->Command);
				removePNode(Current->PID);
			}
		}
	
		m_InputLine = readline(Prompt);
		InputLineCopy = (char*)malloc(strlen(m_InputLine)*sizeof(char));	/* allocate memory for a copy of the input line */
		if (strcmp(m_InputLine, "exit") == 0) {
			Destroy();
			return 0;
		}
		if (strcmp(m_InputLine, "") == 0)
			continue;
		else {
			/* Tokenize the whole input line by spaces 	*/
			iArgsCount = TokenizeLine(options, m_InputLine, InputLineCopy);
					
			/* if the input line is empty 			*/
			if (options[0] != NULL)
				Command = options[0];
			
			/* Call this function to determine which 
			   command will get executed			*/		
			ErrCheck = ParseCommand(Command, options, iArgsCount, InputLineCopy);
			if (ErrCheck > 0)
				exit(1);
		}
		free(InputLineCopy);
		free(options);
	}
    return 0;
}

void ProcessChanged()
{
	/* This function will update the current process List */
	PNode* Current = m_Head;
	int status;
	
	Current = m_Head;
	
	/* Get the sender's PID */
	pid_t pChild = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED);		

	for (;Current != NULL;Current = Current->Next) {
		if (WIFEXITED(status) && strcmp(Current->Status, "finished") != 0 && pChild == Current->PID) {
			strcpy(Current->Status, "finished");
		}
		else if (WIFSTOPPED(status) && strcmp(Current->Status, "stopped") != 0 && pChild == Current->PID) {
			strcpy(Current->Status, "stopped");
		}
		else if (WIFSIGNALED(status) && strcmp(Current->Status, "finished") != 0 && pChild == Current->PID) {
			if (WTERMSIG(status) == SIGKILL)
				strcpy(Current->Status, "finished");
			else if (strcmp(Current->Status, "stopped") != 0) 
				strcpy(Current->Status, "running");
		}
		else if (WIFCONTINUED(status) && strcmp(Current->Status, "running") != 0 && pChild == Current->PID) {
			strcpy(Current->Status, "running");
		}
	}
}

int ParseCommand(char* Command, char* options[], int iArgsCount, char* InputLineCopy)
{
	/* cd option */
	if (strncmp(Command, "cd", strlen(Command)) == 0) {
		if (iArgsCount == 1) {
			char *TildeReplace = (char*)malloc(sizeof(char)*LINESIZE);
			strcpy(TildeReplace, "/home/");			
			strcat(TildeReplace, getlogin());
			strcat(TildeReplace, "/");
			cd(TildeReplace);				/* There was no argument to CD so go to ~	*/
			free(TildeReplace);
		}
		else
			cd(options[1]);
	}
	/* bg option */ 
	else if (strncmp(Command, "bg", strlen(Command)) == 0) {
		if (iArgsCount > 1)
			bg(options);
		else
			printf("RSI: bg: Must supply an argument\n");
	}
	/* bgstart option */
	else if (strncmp(Command, "bgstart", strlen(Command)) == 0) {
		if (iArgsCount > 1)
			bgstart(atoi(options[1]));
		else
			printf("RSI: bgstart: Must supply an argument\n");
	}
	/* bgkill option */
	else if (strncmp(Command, "bgkill", strlen(Command)) == 0) {
		if (iArgsCount > 1) 
			bgkill(atoi(options[1]));
		else
			printf("RSI: bgkill: Must supply an argument\n");

	}
	/* bglist option */
	else if (strncmp(Command, "bglist", strlen(Command)) == 0) {
		bglist();
	}
	/* bgstop option */
	else if (strncmp(Command, "bgstop", strlen(Command)) == 0) {
		if (iArgsCount > 1) 
			bgstop(atoi(options[1]));
		else
			printf("RSI: bgstop: Must supply an argument\n");
	}	
	/* bgstart option */
	else if (strncmp(Command, "", strlen(Command)) == 0) {
		if (iArgsCount > 1)
			bgstart(atoi(options[1]));
		else
			printf("RSI: bgstart: Must supply an argument\n");
	}
	else
		Exec(Command, options);					/* Run an executable from /bin/			*/
	return 0;
}

int bgkill(pid_t PID)
{
	
	int errCheck;
	if (PID <= 0) { 
		printf("RSI: bgkill: %d argument must be a pid\n", PID);
		return 1;
	}
	errCheck = kill(PID, SIGKILL);
	if (errCheck != 0) {
		printf("bgkill: %s\n", strerror(errno));
		return 1;
	}
	return 0;
}

int bgstop(pid_t PID)
{
	int errCheck;
	if (PID <= 0) {
		printf("RSI: bgstop: %d argument must be a pid\n", PID);
		return 1;
	}
	errCheck = kill(PID, SIGSTOP);
	if (errCheck != 0) {
		printf("bgstop: %s\n", strerror(errno));
		return 1;
	}
	return 1;	
}

int bgstart(pid_t PID)
{
	int errCheck;
	if (PID <= 0) {
		printf("RSI: bgstart: %d argument must be a pid\n", PID);
		return 1;
	}
	errCheck = kill(PID, SIGCONT);
	if (errCheck != 0) {
		printf("bgstart: %s\n", strerror(errno));
		return 1;
	}
	return 1;	
}

int bglist()
{
	PNode* Current = m_Head;
	int i;
	for (i = 0;Current != NULL;Current = Current->Next) {
		printf("[%d] %d: %s %s\n", Current->iNumber, Current->PID,
			Current->Status, Current->Command);
		i++;
	}
	printf("Total background job(s):   %d\n", i);
	return 0;	
}

int bg(char* options[]) 
{
	char* NodeCommand = (char*)malloc(sizeof(char)*LINESIZE);
	strcpy(NodeCommand, "");
	
	char** optionsPTR = options;
	*optionsPTR++;
	char* path = (char*)malloc(sizeof(char)*LINESIZE);
	
	strcpy(path, "/bin/");
	strcat(path, optionsPTR[0]);
	
	int i;
	for (i = 0;optionsPTR[i] != NULL;i++) {
		strcat(NodeCommand, optionsPTR[i]);
		strcat(NodeCommand, " ");
	}
	
	int status, err;
	pid_t ChildPID, WaitErr;
	
	ChildPID = fork();
	if (ChildPID < 0) {
		/* There is an error */
		printf("Error forking the process.\n");
		exit(1);
	}
	if (ChildPID >= 0) {
		if (ChildPID == 0) {
			err = execvp(optionsPTR[0], optionsPTR);
			printf("RSI: %s\n", strerror(errno));
			exit(1);
		}
		else {
			addPNode(NodeCommand, ChildPID, m_Head, "running", &status);
			free(NodeCommand);
			WaitErr = waitpid(ChildPID, &status, WNOHANG);
		}
	}
	free(path);
	return 0;	
}

/*
 * Takes a string and calls chdir with that 
 * as the path.  If chdir fails, outputs the 
 * error and then returns, otherwise the current
 * directory is changed.
 * param char*<arg>: The input path.
 */
int cd(char* arg)
{
	/* CD only cares about options[1], it only takes 1 arg *
	 * First check through "arg" for instances of a        *
	 * special character such as "~".		       */ 
	int myErrNo;
	myErrNo = chdir(arg);						/* Make the system call to chdir */
	if (myErrNo == -1) {
		/* call failed */
		switch (errno) {
			printf("cd : %s\n", strerror(errno));
			return 1;
		}
	}	
	return 0;	
}

int Exec(char* Command, char* options[])
{
	int status, err;
	pid_t ChildPID, WaitErr;
	char* path = (char*)malloc(sizeof(char)*LINESIZE);
	strcpy(path, "/bin/");
	strcat(path, Command);
	ChildPID = fork();
	if (ChildPID < 0) {
		/* There is an error */
		printf("Error forking the process.\n");
		exit(1);
	}
	if (ChildPID >= 0) {
		if (ChildPID == 0) {
			err = execvp(Command, options);
			printf("RSI: %s\n", strerror(errno));
			exit(1);
		}
		else {
			
			WaitErr = waitpid(ChildPID, &status, WUNTRACED);
		}
	}
	return 0;
}

int Destroy()
{
	PNode* Current = m_Head; pid_t HeadPID; int errCheck; PNode * Prev;
	if (Current == NULL)
		return 0;
	/* Kill all the processes */
	for (;Current != NULL;Current = Current->Next) {
		errCheck = kill(Current->PID, SIGKILL);
		if (errCheck != 0) {
			printf("%s\n", strerror(errno));
			return 1;
		}
	}
	Current = m_Head;
	Prev = Current;
	for (HeadPID = Current->PID;Current != NULL;Current = Current->Next) {
		if (Current->Next == NULL) {			
			if (Current->PID == HeadPID) {		/* Got to the start of the list again */
				break;
			}
			free(Current);
			Prev->Next = NULL;
			Prev = Current;
			Current = m_Head;
		}
		Prev = Current;
	}
	return 0;
}

/*
 * Function takes an input line and then splits it
 * into char* tokens delimited by spaces.
 * param char*[]<options>: The token array to edit.
 * param char*<InputLine>: The Input Line to Parse.
 */
int TokenizeLine(char* options[], char* InputLine, char* InputLineCopy)
{
	char *current, *TildeReplace, *temp;				/* TildeReplace is used for replacing the "~" 	*/
	temp = (char*)malloc(sizeof(char)*LINESIZE);
	TildeReplace = (char*)malloc(sizeof(char)*LINESIZE);
	strcpy(TildeReplace, "/home/");			
	strcat(TildeReplace, getlogin());
	strcat(TildeReplace, "/");
	
	int optionsPosition = 0;
	
	strcpy(InputLineCopy, InputLine);
	
	current = strtok(InputLine, " ");
	if (current != NULL) {
		options[optionsPosition] = current;
		optionsPosition++;
		for (;;) {		
			current = strtok(NULL, " ");
			/* account for the "~" special character, this character can
			   be replaced by "/home/%currUser%/" */
			if (current != NULL) {
				current = str_replace("~", TildeReplace, current, temp);
				options[optionsPosition] = current;
				optionsPosition++;
			}
			else {
				options[optionsPosition] = NULL;
				break;
			}
		}
	}
	return optionsPosition;				/* returns the number of arguments. 		*/
}

/* Adds a node to the PNode* List */
int addPNode(char* Command, pid_t PID, PNode *Current, char* Status, int *iStatus)
{	
	/* Allocate and initialize */
	PNode* newNode = (PNode*)malloc(sizeof(PNode));
	newNode->Command = (char*)malloc(sizeof(char)*LINESIZE);
	newNode->Status = (char*)malloc(sizeof(char)*LINESIZE);
	
	strcpy(newNode->Command, Command);
	strcpy(newNode->Status, Status);
	newNode->PID = PID;
	newNode->iStatus = iStatus;
	newNode->Next = NULL;
	
	if (Current == NULL) {
		newNode->iNumber = 1;
		m_Head = newNode;
		return 1;
	}
	
	int i;
	for (i = 2;;i++) {
		if (Current->Next != NULL)
			Current = Current->Next;
		else
			break;
	}
	newNode->iNumber = i; 
	Current->Next = newNode; /* Add the new node on the end */
	
	/* on success, return # of nodes */
	return (i);	
}

/* This function always starts from the beginning of the process list. */
int removePNode(pid_t PID) 
{
	PNode* Current = m_Head;
	
	if (Current == NULL)
		return -1;	/* Error */
	
	if (m_Head->PID == PID) {
		/* the head node is the one to remove */
		if (m_Head->Next != NULL)
			m_Head = m_Head->Next;
		else						/* Head is the only node */
			m_Head = NULL;
	}
	else {
		for (;;) {
			if (Current->Next != NULL) {
				if (Current->Next->PID == PID) {
					/* This is the node to remove */
					Current->Next = Current->Next->Next;
					return 0;
				}
			}
			else
				break;
		}
	}
	return 0;	
}

/* 
 * This is an adaptation of the code found 
 * at http://www.planet-source-code.com/vb/scripts/ShowCode.asp?txtCodeId=10890&lngWId=3
 * Searches through the "TargetString" for instances of "ToBeReplaced" and 
 * then replaces them.  It returns a pointer to the new string, and requires that
 * Outputstring is already allocated.  The caller has to free the allocated memory.
 */
char *str_replace(char * ToBeReplaced, char * ToReplace, char * TargetString, char* OutputString){
	char*t4;
	int ContainedString = 0;	
	strcpy(OutputString, "");
	t4 = OutputString;
	while(strstr(TargetString,ToBeReplaced)){
		ContainedString = 1;
		t4=strstr(TargetString,ToBeReplaced);
		strncpy(OutputString+strlen(OutputString),TargetString,t4-TargetString);
		strcat(OutputString,ToReplace);
		t4+=strlen(ToBeReplaced);
		TargetString=t4;
	}
	if (ContainedString == 1) {
		return strcat(OutputString,t4);
	}
	else {
		return TargetString;
	}
}
