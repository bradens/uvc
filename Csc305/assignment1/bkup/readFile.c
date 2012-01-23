/***************************
 * Read input file which specifies triangles vertices & colors
 *
 * Copyright Amy Gooch 2003
 ***************************/
#include "readFile.h"


int read_file( char *fileName) {
  FILE *f;
  int result;
  char temp[256];
  char command[32];
  int a,b,c;

  f = fopen(fileName, "r");

  if (f == NULL){
         printf( "\n*******************************************************\n");
       printf(" ERROR: Unable to open file: %s\n", fileName);
           // in case of invalid file
       printf( "\n*******************************************************\n");
       return(0);
  }
  else printf("File Opened: %s\n", fileName);

  result = fscanf(f, "%[^\n]\n", &temp);//read first line
  if (result == EOF) printf("End of file\n");

  while (result != EOF){
    if (temp[0] != '#'){
      if (temp[0] == 'b'){
	beginTriangle();
      }//end of if begin
      if (temp[0] == 'c'){
	 sscanf(temp, "%s%d%d%d", command,&a,&b,&c);
	 setColor3(a,b,c);
      }//end of if color
      if (temp[0] == 'v'){
	 sscanf(temp, "%s%d%d", command,&a,&b);
	 setVertex(a,b);
      }//end of if vertex
      if (temp[0] == 'e'){
	endTriangle();
      }//end of end

    }//if haven't found a comment '#'
    strcpy(temp, ""); //reset temp string
    strcpy(command, "");//reset command string
    //read next line
    result = fscanf(f,"%[^\n]\n",&temp);
  }//end of while not EOF

  printf("Data Processed, File Closed\n");
  fclose(f);
}

