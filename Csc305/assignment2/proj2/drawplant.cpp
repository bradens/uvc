#include <assert.h>
#include <math.h>
#include <GL/glut.h>
#include "drawplant.h"
#include "Mat.h"
#include <list>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

using namespace std;

int ITER=0; //number of iterations to go thru 
list<Mat> stack;
Mat currentMat;
float r = 1/sqrt(2);
float R = sqrt(2);
float scale = 1;
float angle = M_PI/6; // Turning angle is pi/6


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

void load3DMat(Mat mat)
{
	GLfloat M[16];

	M[0] = mat.data[0]; M[1] = mat.data[4]; M[2] = mat.data[8]; M[3] = mat.data[12];
	M[4] = mat.data[1]; M[5] = mat.data[5]; M[6] = mat.data[9]; M[7] = mat.data[13];
	M[8] = mat.data[2]; M[9] = mat.data[6]; M[10] = mat.data[10]; M[11] = mat.data[14];
	M[12] = mat.data[3]; M[13] = mat.data[7]; M[14] = mat.data[11]; M[15] = mat.data[15];

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(M);
}


void draw(int i)
{
	if (i == 0)
		drawLeaf();
	else
	{
		Mat newMat = Mat();
		newMat = newMat * r;
		scale *= r;
		currentMat = currentMat * newMat;
		drawTwig(i-1);

		stack.push_front(currentMat);
		glPushMatrix();

		Mat cwTurn = Mat();
		cwTurn.turnRight(angle);
		currentMat = currentMat * cwTurn;
		draw(i-1);
		currentMat = stack.front();
		stack.pop_front();
		glPopMatrix();
		stack.push_front(currentMat);
		glPushMatrix();

		Mat ccwTurn = Mat();
		ccwTurn.turnLeft(angle);
		currentMat = currentMat * ccwTurn;
		draw(i-1);

		currentMat = stack.front();
		stack.pop_front();
		glPopMatrix();
	}
}

void drawTwig(int i) {
	if (i == 0)
	{
		load3DMat(currentMat);
		glColor3f(0.33, 0.4, .01);
		glBegin(GL_POLYGON);
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, 6.0);
		glVertex2f(1.0, 6.0);
		glVertex2f(1.0, 0.0);
		glEnd();

		// Translate the length of the stick
		Mat newMat = Mat();
		newMat.setTranslation(0, 6, 0, 1);
		currentMat = currentMat * newMat;
	}
	else
	{
		Mat newMat = Mat();
		newMat.scale(R);
		scale *= R;
		currentMat = currentMat * newMat;
		drawTwig(i-1);
	}
	Mat newMat = Mat();
	newMat.scale(1/scale);
	currentMat = currentMat * newMat;
	scale = 1;
}

void drawLeaf(void) {
	load3DMat(currentMat);
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

void drawSquare(void) {
	load3DMat(currentMat);
	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0,0.0);
	glVertex2f(2.0, 0.0);
	glColor3f(0.4, 0.4, 0.4);
	glVertex2f(1.0,1.0);

	glColor3f(0.8, 0.8, 0.8);
	glVertex2f(2.0,0.0);
	glVertex2f(2.0, 2.0);
	glColor3f(0.4, 0.4, 0.4);
	glVertex2f(1.0,1.0);

	glColor3f(0.8, 0.8, 0.8);
	glVertex2f(2.0,2.0);
	glVertex2f(0.0, 2.0);
	glColor3f(0.4, 0.4, 0.4);
	glVertex2f(1.0,1.0);

	glColor3f(0.8, 0.8, 0.8);
	glVertex2f(0.0,2.0);
	glVertex2f(0.0, 0.0);
	glColor3f(0.4, 0.4, 0.4);
	glVertex2f(1.0,1.0);

	glEnd();
}

void drawbg()
{
	Mat newMat = Mat();
	load3DMat(newMat);
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(-300.0, 100.0);
	glVertex2f(300.0, 100.0);
	glColor3f(0.0f, 0.5f, 0.0f);
	glVertex2f(300.0, 0.0);
	glVertex2f(-300.0,0.0);
	glEnd();
}

void drawPlant(void) {

	drawbg();
	currentMat.identity();
	currentMat.setTranslation(0,10,0,1);
	draw(ITER);
}
