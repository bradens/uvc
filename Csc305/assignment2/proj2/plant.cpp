#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include "drawplant.h"

/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=1000;  /* window width */
int H=800;  /* window height */

/* local function declarations */
void display(void);
void init(void);
void keyboard (unsigned char key, int x, int y);

int main (int argc, char** argv) {
  int win;

  glutInit(&argc,argv);
 
  extern int ITER;
  if (argc ==2){
    ITER = atoi(argv[1]);
    printf("Plant will iterate over %d steps\n", ITER);
  }


  glutInitWindowSize(W,H);
  glutInitWindowPosition(100,100);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  win = glutCreateWindow("plant");
  glutSetWindow(win);
  glutKeyboardFunc(keyboard);
  init();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}


void keyboard (unsigned char key, int x, int y){
  GLint params[2];
  extern int ITER;
  switch(key){
  case '.':
    //Use keyboard to increment number of iterations
    ITER++;
    printf("Redraw with plant iteration = %d\n", ITER);
    display();
    break;
  case ',':
    //Use keyboard to decrement number of iterations
    if (ITER > 0)
      ITER--;
    printf("Redraw with plant iteration = %d\n", ITER);
    display();
    break;
  case 'q':
    exit(0);
    break;
  default:
    break;
  }
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //create a viewing volume, see pg 124 of OGL Programming book (Version1.1)
  //note: numbers should be proportional to window size
  glOrtho(-50.0/*left*/, 50.0/*right*/, 0.0/*bottom*/, 80.0/*top*/, -1.0/*near*/, 1.0/*far*/);
  //thus the center in screen x is 0, bottom in y is 0, top is 80;
}


void display() {

  glClear(GL_COLOR_BUFFER_BIT);

  /* put plant drawing code here */

  drawPlant();

  /* end drawing code */

  glFlush();
}









