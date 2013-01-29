
#include "readFile.h"

char inputFile[128];
int USE_INPUT_FILE =0;

int W=500;  /* width of window */
int H=400;  /* height of window */

/* local function declarations */
void display(void);
void init(void);
void keyboard (unsigned char key, int x, int y);

int main (int argc, char** argv) {
  int win;

  //if was given a file in the command line, set up variable with name of file
  if (argc>1){
    strcpy(inputFile, argv[1]);
    USE_INPUT_FILE =1;
  }
  glutInit(&argc,argv);
  glutInitWindowSize(W,H);
  glutInitWindowPosition(100,100);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  win = glutCreateWindow("Triangle");
  glutSetWindow(win);
  glutKeyboardFunc(keyboard);
  init();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}


void keyboard (unsigned char key, int x, int y){
  GLint params[2];
  switch(key){
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
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

/* Called by glut to draw contents of window */
void display() {

  /* erase the (real-life) frame buffer */
  glClear(GL_COLOR_BUFFER_BIT);

  /* drawing code */
  initFakeFrameBuffer(W,H); 

  if ( USE_INPUT_FILE ){
    //if an input file was specified render it
    read_file(inputFile);
  }
  else {

    beginTriangle();
    setColor3(200,128,128); /* Rose */
    setVertex(250,100);
    setColor3(200,128,255); /* Rose */
    setVertex(200,300);
    setColor3(200,128,0); /* Rose */
    setVertex(100,100);
    endTriangle();
    
    beginTriangle();
    setColor3(8,128,250); /* Light Blue */
    setVertex(250,100);
    setColor3(0,128,0); /* Rose */
    setVertex(400,300);
    setColor3(0,128,255); /* Rose */
    setVertex(200,300);
    endTriangle();
  }
  
  outputFrame();
  /* end drawing code */
  
  glFlush();
}









