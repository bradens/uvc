#include <assert.h>
#include <math.h>
#include <GL/glut.h>
#include "drawplant.h"
// commented out until you implement it
// #include "myMatrix.h"
#include <stdio.h>

int ITER=0; //number of iterations to go thru 



/* Takes a 2D matrix in row-major order, and loads the 3D matrix which
   does the same trasformation into the OpenGL MODELVIEW matrix, in
   column-major order. */
void load2DMatrix(GLdouble m00, GLdouble m01, GLdouble m02,
		  GLdouble m10, GLdouble m11, GLdouble m12,
                  GLdouble m20, GLdouble m21, GLdouble m22) {

  GLfloat M3D [16];  /* three dimensional matrix doing same transform */

  M3D[0] = m00;  M3D[1] = m10; M3D[2] = 0.0; M3D[3] = m20;
  M3D[4] = m01;  M3D[5] = m11; M3D[6] = 0.0; M3D[7] = m21;
  M3D[8] = 0.0;  M3D[9] = 0.0; M3D[10] = 1.0; M3D[11] = 0.0;
  M3D[12] = m02; M3D[13] = m12; M3D[14] = 0.0; M3D[15] = m22;

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(M3D);
}

void drawLeaf(void) {
  glColor3f(0.1,0.9,0.1); 
  glBegin(GL_POLYGON);
  glVertex2f(0.0,0.0);
  glVertex2f(1.0,0.7);
  glVertex2f(1.3,1.8);
  glVertex2f(1.0,2.8);
  glVertex2f(0.0,4.0);
  glVertex2f(-1.0,2.8);
  glVertex2f(-1.3,1.8);
  glVertex2f(-1.0,0.7);
  glEnd();
}

void drawPlant(void) {

   load2DMatrix(sqrt(3.0)/2.0, -1.0/2.0,      0.0,
 	       1.0/2.0,       sqrt(3.0)/2.0, 4.0,
 	       0.0,           0.0,           1.0);


   drawLeaf();

}
