#include <GL/glut.h>
#include <iostream>

using namespace std;

int win;

void display() 
{
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char **argv)
{
	cout << "test" << endl;
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100,100);
	win =  glutCreateWindow(argv[0]);
	glutSetWindow(win);
	init();
	glutDisplayFunc(display);
	glutMainLoop();
}


