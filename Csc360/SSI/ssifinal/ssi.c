/*
 * Programming Assignment 0
 * Csc 360 A01
 * Braden Simpson, V00685500
 * ssi.c
 */
#include "ssi.h"
struct node {
	char* name;
	struct node* LeftChild;
	struct node* RightSibling;
	struct node* RootNode;
};
/* Options is for handling the input after a command, ex. ls -r */
char* options;	
/* ptr is the variable for the line of input entered by the user */
char ptr[LINESIZE];
/* hold the string of the full path from the root */
char *FullPath;
/* holds the temporary string of the full path */
char *TempPath;
/* root is the pointer to the root node of the tree */
item* root;
/* The current level of the tree that the user is on, ex. starts at root. */
item* CurrentNode;
/* The tokens delimited by / */
char** tokens;
/* Token Counter */
int tokenCount;

int main(int argc, char** argv)
{	
	root = (item*)malloc(sizeof(item)); /* Initialize the root node */
	root->name = "/";
	root->LeftChild = NULL;
	root->RightSibling = NULL;
	root->RootNode = NULL;
	TempPath = (char*)malloc(LINESIZE*(sizeof(char*))); /* allocate memory for TempPath */
	CurrentNode = root;
	FullPath = CurrentNode->name;
	strcpy(TempPath, FullPath);
	char* InputLine;
	while (1) {	/* The main command loop */
		printf("SSI %s $ ", FullPath);
		/* Read the line */
		InputLine = readLine();
		/* check if the line is NULL */
		if (InputLine[0] != '\0') {
			TempPath = (char*)malloc(LINESIZE*(sizeof(char*))); /* allocate memory for TempPath */
			strcpy(TempPath, FullPath);
			options = (char*)malloc(sizeof(char)*LINESIZE);
			/* Parse the token and then call the appropriate functions */
			ParseToken(InputLine);
			TempPath = NULL;
			free(TempPath);
			free(tokens);
			free(options);
			options[0] = '\0';
		}
	}
	return 0;
}

/* 
 * This function takes in a line of input and parses the tokens
 * in the input and calls the appropriate functions.
 * param<line> : The whole line given by the user with no \n
 * return<int> : Returns 1 if error.
 */
int ParseToken(char* line)
{	
	tokenizeLine(line);
	char* token = options;
	/* exit */
	if (strcmp(token, "exit") == 0) {
		dispose();
		exit(0);
	}
	/* the LS option */
	else if (strcmp(token, "ls") == 0) {
		token = strtok(NULL, " ");
		if (token != NULL) {
			options = token;
		}
		if (tokens != NULL) {
			if (strncmp(tokens[0], "-R", strlen(tokens[0])) == 0) {
				DoLSRecursive(CurrentNode);
			}
			else {
				item* node = FindNode(options);
				if (node != NULL)
					DoLS(NULL, 1, node);
				return 0;
			}
		}
		else
			DoLS(options, 1, CurrentNode);	
	}
	/* the rmdir option */
	else if (strcmp(token, "rmdir") == 0) {
		token = strtok(NULL, " ");
		if (token != NULL) {
			options = token;
		}
		if (tokens != NULL)
			DoRmdir();
		else {
			return 0;
		}
	}
	/* the Mkdir option */
	else if (strcmp(token, "mkdir") == 0) {
		token = strtok(NULL, " ");
		if (token != NULL) {
			options = token;
		}
		if (tokens != NULL) {
			DoMkdir(options);
		}
		else {
			return 0;
		}
	}
	/* the Cd option */
	else if (strcmp(token, "cd") == 0)
	{
		token = strtok(NULL, " ");
		if (token != NULL) {
			options = token;
		}
		if (tokens != NULL) {
			DoCd(options);
		}
		else {
			return 0;
		}
	}
	else {
		printf("%s: command not found\n", token);
		return 0;
	}
	return 0;
}
/*
 * Tokenizes the input line by "/" and inserts the tokens 
 * into a char* array.
 * param<line> : The input line from the command line.
 */
int tokenizeLine(char* line)
{
    tokens = (char**)malloc(sizeof(char*)*LINESIZE);
	tokenCount = 0;
	char* token = strtok(line, " ");
	strcpy(options, token);
	token = strtok(NULL, "/");
	int i;
	if (token != NULL) {
		for (i = 0;token != NULL;token = strtok(NULL, "/")) {
			tokens[i] = token;
			i++;
		}
		tokenCount = i;
		return 0;
	}
	else
	{
		tokens = NULL;
		return 0;
	}
}
/*
 * Finds the node where node->RightSibling = the input node.
 * param<node> : Struct pointer to the leftmost child on the Current
 * level.
 * returns<item*> : Struct pointer to the node found. NULL if not found.
 */
item* FindNodePrevious(item* node, char* name)
{
	if (node->RightSibling != NULL) {
		if (strncmp(node->RightSibling->name, name, strlen(name)) == 0) {
			return node;
		}
		return FindNodePrevious(node->RightSibling, name);
	}
	else
		return FindNodePrevious(node->RightSibling, name);
}
/*
 * Function to take in a given node and finds it's
 * right-most sibling.
 */
item* FindRightMostSibling(item* node) 
{
	if (node->RightSibling == NULL)
		return node;
	else 
		return FindRightMostSibling(node->RightSibling);
}
/*
 * Function to take in a node and search for another node
 * by iterating from left -> Right
 * if the node is not found it return NULL, else returns the
 * node 
 */
item* FindNodeOnThisLevel(item* node, char* token)
{
	if (node == NULL)
		return node;
	if (strncmp(node->name, token, strlen(token)) == 0)
		return node;
	else
		return FindNodeOnThisLevel(node->RightSibling, token);
}
/*
 * Function to handle when the user enters the mkdir command
 */
int DoMkdir(char* options)
{
	/* child is the new node to be added */
	item* child = (item*)malloc(sizeof(item));
	child->RightSibling = NULL;
	child->LeftChild = NULL;
	child->RootNode = NULL;
	
	/* searchNode and searchNode parent are used for traversals */
	item* searchNode = CurrentNode->LeftChild;
	item* searchNodeParent = CurrentNode;
	item* LevelNode = searchNode;
	
	/* token and nextToken are strings from tokens** */
	char *token = NULL;
	char *nextToken = NULL;
	
	int tokenIndex = 0;
	if (tokenCount > 0)
		token = tokens[tokenIndex];
	if (tokenCount > 1) {
		tokenIndex++;
		nextToken = tokens[tokenIndex];
	}
	/* There is no other nodes at this level and the next token is NULL and token != "." or ".." */
	if (CurrentNode->LeftChild == NULL && 
	    !(strncmp(token, "..", strlen(token)) == 0) && 
            nextToken == NULL &&
            !(strncmp(token, ".", strlen(token)) == 0)) {
		CurrentNode->LeftChild = child;
	}
	else {	
		while (1) {
			/* check for the node with the name <token> on this level */
			searchNode = FindNodeOnThisLevel(LevelNode, token);

			/* if the token is "." and the next token is !NULL then stay on same level */
			if (strncmp(token, ".", strlen(token)) == 0 && nextToken != NULL) {				
				if (searchNodeParent->LeftChild != NULL)
					searchNode = searchNodeParent->LeftChild;
				else 
					searchNode = searchNodeParent;			
				tokenIndex++;
				token = nextToken;
				if (tokenIndex < tokenCount)
					nextToken = tokens[tokenIndex];
				else 
					nextToken = NULL;
			}
			/* if the token is ".." and the nextToken != NULL then go back up one level */
			else if (strncmp(token, "..", strlen(token)) == 0 && nextToken != NULL) {
				if (searchNodeParent->RootNode != NULL) {
					searchNode = searchNodeParent;
					searchNodeParent = searchNodeParent->RootNode;
					LevelNode = searchNode;
					tokenIndex++;
					token = nextToken;
					if (tokenIndex < tokenCount)
						nextToken = tokens[tokenIndex];
					else 
						nextToken = NULL;
				}
				else {
					printf("mkdir: cannot create directory '%s': No such file or directory\n", token);
					free(child);
					return 0;
				}
			}
			/* if the node wasnt found and there is another token, output error */
			else if (searchNode == NULL && nextToken != NULL) {
				printf("mkdir: cannot create directory: No such file or directory '%s'\n", token);
				free(child);
				return 0;
			}
			/* if the node is found and the next token is NULL output error */
			else if (searchNode != NULL && nextToken == NULL) {
				printf("mkdir: cannot create directory '%s': File exists\n", token);
				free(child);
				return 0;
			}
			/* if the node is found and the next token is not NULL, go down one level */
			else if (searchNode != NULL && nextToken != NULL) {
				searchNodeParent = searchNode;
				LevelNode = searchNode->LeftChild;
				/* searchNode = LevelNode; */
				tokenIndex++;
				token = nextToken;
				if (tokenIndex < tokenCount)
					nextToken = tokens[tokenIndex];
				else 
					nextToken = NULL;
			}
			/* if the node is not found and the next token is NULL, then make the directory */
			else if (searchNode == NULL && nextToken == NULL) {
				if (strcmp(token, "..") == 0) {
					printf("mkdir: cannot create directory '%s': File exists\n", token);
					return 0;
				}
				if (strcmp(token, ".") == 0) {
					printf("mkdir: cannot create directory '%s': File exists\n", token);
					return 0;
				} 				
				if (LevelNode != NULL) {
					searchNode = FindRightMostSibling(LevelNode);
					searchNode->RightSibling = child;
				}
				break;
			}
			if (searchNode == NULL)
				break;
		}
	}	
	/* allocate memory for the string which will be copied */
	child->name = (char*)malloc(sizeof(char)*LINESIZE);
	strcpy(child->name, token);
	if (searchNodeParent->LeftChild == NULL)
		searchNodeParent->LeftChild = child;
	child->RootNode = searchNodeParent;
	return 0;
}
/*
 * Function to perform a 
 * rm command on a directory
 * uses the same logic as mkdir for traversing
 * the tree.
 */
int DoRmdir()
{
	char *token = NULL;
	char *nextToken = NULL;
	int tokenIndex = 0;
	if (tokenCount > 0)
		token = tokens[tokenIndex];
	if (tokenCount > 1) {
		tokenIndex++;
		nextToken = tokens[tokenIndex];
	}	
	item* searchNode = CurrentNode->LeftChild;
	item* LevelNode = searchNode;
	item* searchNodeParent = CurrentNode;

	/* iterating through the tokens separated by /'s */
	while (1) {
		searchNode = FindNodeOnThisLevel(LevelNode, token);
		if (strncmp(token, "..", strlen(token)) == 0 && nextToken == NULL) {
			if (searchNodeParent->RootNode == NULL) {
				printf("rmdir: cannot navigate to '..'.\n");
				return 0;
			}
			else {
				CurrentNode = searchNodeParent->RootNode;
				return 0;
			}
		}
		else if (strncmp(token, ".", strlen(token)) == 0 && nextToken == NULL) {
			printf("rmdir: failed to remove '%s'", token);
			return 0;
		}
		else if (strncmp(token, "..", strlen(token)) == 0 && nextToken != NULL) {
			if (searchNodeParent->RootNode != NULL) {
				searchNode = searchNodeParent;
				searchNodeParent = searchNodeParent->RootNode;
				LevelNode = searchNode;
				tokenIndex++;
				token = nextToken;
				if (tokenIndex < tokenCount)
					nextToken = tokens[tokenIndex];
				else
					nextToken = NULL;
			}
			else {
				printf("rmdir: %s: No such file or directory\n", token);
				return 0;
			}
		}
		else if (searchNode == NULL && nextToken != NULL) {
			printf("rmdir: %s: No such file or directory\n", token);
			return 0;
		}
		else if (searchNode != NULL && nextToken == NULL) {
			/* directory is deleteable */
			if (searchNode->LeftChild == NULL) {
				/* directory to be deleted is the leftchild of the parent */
				if (strncmp(searchNodeParent->LeftChild->name, searchNode->name, strlen(searchNode->name)) == 0) {
					/* point the parent's leftchild to the searchnodes sibling */
					if (searchNode->RightSibling != NULL) {
						searchNodeParent->LeftChild = searchNode->RightSibling;
						return 0;
					}
					else {
						searchNodeParent->LeftChild = NULL;
						return 0;
					}
				}
				else {
					searchNode = FindNodePrevious(LevelNode, searchNode->name);
					searchNode->RightSibling = NULL;
					return 0;
				}
			}
			else {
				printf("rmdir: failed to remove '%s': Directory not empty\n", token);
				return 0;
			}
		}
		else if (searchNode != NULL && nextToken != NULL) {
			searchNodeParent = searchNode;
			LevelNode = searchNode->LeftChild;
			/* searchNode = LevelNode; */
			tokenIndex++;
			token = nextToken;
			if (tokenIndex < tokenCount)
				nextToken = tokens[tokenIndex];
			else
				nextToken = NULL;
		}
		else if (searchNode == NULL && nextToken == NULL) {
			printf("rmdir: %s: No such file or directory\n", token);
			return 0;
		}
		if (searchNode == NULL)
			break;
	}
	return 0;
}
/*
 * Function that performs a cd.
 * Will edit CurrentNode.
 * calls FindNode()
 */
int DoCd(char* options)
{	
	item* node = FindNode(options);
	if (node != NULL)
	{
		CurrentNode = node;
		FullPath = TempPath;
		if (strcmp(CurrentNode->name, "/") == 0) {
			strcpy(TempPath, "/");
			FullPath = TempPath;
		}
	}
	return 0;
}
/* 
 * Does a recursive ls from the input node.
 * param<item* node> : the input node to do the ls from
 */
int DoLSRecursive(item* node)
{
	printf("%s:\n", node->name);
	DoLS(NULL, 1, node);
	
	if (node->LeftChild != NULL) {
		DoLSRecursive(node->LeftChild);
	}
	if (node->RightSibling != NULL) {
		DoLSRecursive(node->RightSibling);
	}
	return 0;
}
/* 
 * Function to handle when the user enters the ls command
 * param<char* options> : the command.
 * param<int firstCall> : if 1, the function will print the LeftChild
 * param<item* LevelNode> : The node from which to start the LS
 * returns 1 if failure.
 */
int DoLS(char* options, int firstCall, item* LevelNode)
{
		item* node = LevelNode;
		if (firstCall == 1) {
			if (LevelNode->LeftChild == NULL) {	
				printf(".\t..\n");
				return 0;
			}
			node = node->LeftChild;
			printf(".\t..\t%s\t", node->name);
			if (node->RightSibling != NULL)
				DoLS(options, 0, node->RightSibling);
		}
		else if (firstCall == 0) {
			printf("%s\t", node->name);
			if (node == NULL)
				return 0;
			if (node->RightSibling != NULL)
				DoLS(options, 0, node->RightSibling);
		}
		if (firstCall == 1)
			printf("\n");
	return 0;	
}
/*
 * Function which finds a node in the whole tree
 * works by using tokens, and outputs according to the commands.
 * uses almost the same logic as mkdir and rmdir.  Used by CD and LS
 */
item* FindNode(char* command)
{
	char *token = NULL;
	char *nextToken = NULL;
	int tokenIndex = 0;
	
	if (tokenCount > 0)
		token = tokens[tokenIndex];
	if (tokenCount > 1) {
		tokenIndex++;
		nextToken = tokens[tokenIndex];
	}	
	item* searchNode = CurrentNode->LeftChild;
	item* LevelNode = searchNode;
	item* searchNodeParent = CurrentNode;
	
	/* iterating through the tokens separated by /'s */
	while (1) {
		searchNode = FindNodeOnThisLevel(LevelNode, token);
		/* Going up one level and nextToken doesnt exist */
		if (strncmp(token, "..", strlen(token)) == 0 && nextToken == NULL) {
			if (searchNodeParent->RootNode == NULL) {
				return NULL;
			}
			else {
				/* remove last token */
				int len = strlen(searchNodeParent->name);
				int tempLen = strlen(TempPath);
				TempPath[tempLen-(len+1)] = '\0';
				
				/* if the root directory dont get rid of the / */
				if (strcmp(searchNodeParent->RootNode->name, "/") == 0)
					TempPath = strcat(TempPath, "/");
				return searchNodeParent->RootNode;
			}
		}
		else if (strncmp(token, ".", strlen(token)) == 0 && nextToken == NULL) {
			return searchNodeParent;
		}
		else if (strncmp(token, ".", strlen(token)) == 0 && nextToken != NULL) {
			tokenIndex++;
			token = nextToken;
			if (tokenIndex < tokenCount)
					nextToken = tokens[tokenIndex];
				else
					nextToken = NULL;
			searchNode = LevelNode;
			
		}
		else if (strncmp(token, "..", strlen(token)) == 0 && nextToken != NULL) {
			if (searchNodeParent->RootNode != NULL) {
				/* remove last token */
				int len = strlen(searchNodeParent->name);
				int tempLen = strlen(TempPath);
				TempPath[tempLen-(len+1)] = '\0';
				
				/* if the root directory dont get rid of the / */
				if (strcmp(searchNodeParent->RootNode->name, "/") == 0)
					TempPath = strcat(TempPath, "/");
				
				searchNode = searchNodeParent;
				searchNodeParent = searchNodeParent->RootNode;
				LevelNode = searchNode;
				tokenIndex++;
				token = nextToken;
				if (tokenIndex < tokenCount)
					nextToken = tokens[tokenIndex];
				else
					nextToken = NULL;
			}
			else {
				printf("%s: %s: No such file or directory\n", command, token);
				return 0;
			}
		}
		else if (searchNode == NULL && nextToken != NULL) {
			printf("%s: %s: No such file or directory\n", command, token);
			return 0;
		}
		else if (searchNode != NULL && nextToken == NULL) {
			/* update the path */
			if (!(strcmp(searchNodeParent->name, "/") == 0))
				TempPath = strcat(TempPath, "/");
			
			TempPath = strcat(TempPath, searchNode->name);
			return searchNode;
		}
		else if (searchNode != NULL && nextToken != NULL) {
			/* update path */
			if (!(strcmp(searchNodeParent->name, "/") == 0))
				TempPath = strcat(TempPath, "/");
			TempPath = strcat(TempPath, searchNode->name);
	
			searchNodeParent = searchNode;
			LevelNode = searchNode->LeftChild;
			/*searchNode = LevelNode;*/
			tokenIndex++;
			token = nextToken;
			if (tokenIndex < tokenCount)
				nextToken = tokens[tokenIndex];
			else
				nextToken = NULL;
		}
		else if (searchNode == NULL && nextToken == NULL) {
			printf("%s: %s: No such file or directory\n", command, token);
			return NULL;
		}
		if (searchNode == NULL)
			break;
	}
	if (searchNode->LeftChild != NULL) {
		/* update the path */
		TempPath = strcat(TempPath, "/");
		TempPath = strcat(TempPath, searchNode->name);
	}
	return searchNode;
} 
/*
 * Uses a preorder traversal to go through
 * The whole tree and frees each node. 
 * does NOT free the input node, must remember
 * to free the root node after calling this function
 */
int FreeAllNodes(item* node)
{
	if (node->LeftChild != NULL) {
		FreeAllNodes(node->LeftChild);
		free(node->LeftChild);
	}
	if (node->RightSibling != NULL) {
		FreeAllNodes(node->RightSibling);
		free(node->RightSibling);
	}
	return 0;
}

/*
 * Frees all allocated memory
 * then exits the program.
 */
int dispose()
{
	FreeAllNodes(root);
	free(root);
	free(options);
	free(TempPath);
	free(tokens);
	return 0;
}
/*
 * Wait for the user to enter a line
 * then cut off the carriage return
 * and return it.
 */
char* readLine()
{
	fgets(ptr, LINESIZE, stdin);
	int len = strlen(ptr);
	if (ptr[len-1] == '\n')
		ptr[len-1] = '\0';
	return ptr;
}
