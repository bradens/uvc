#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include "lowlevel.h"

/* Variables local to this file */

 int W; /* width of frame buffer */
 int H; /* height of frame buffer */


color current; /* current color */
vertex* v[3];  /* the array of vertices we store away */
int ind = 0;  /* index of current vertex in the array */

/*****************************************************************************/
/* Fake frame buffer - Use an array of 3 GLubytes per pixel as an imitiation
   frame buffer. We'll color the pixels of this array to draw a triangle */

/* pointer to fake frame buffer */
GLubyte* fb;


/* Allocate storage for fake frame buffer, and fill it with all white */
void initFakeFrameBuffer(int w, int h) {
  int i;

  fb = (GLubyte*) malloc(W*H*3*sizeof(GLubyte));
  for (i=0; i<W*H*3; i++) {
    fb[i]=0x00;
  }
}

/* Draws fake framebuffer into OpenGL window and releases storage */
void outputFrame(void) {
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glRasterPos3f(0.0,0.0,0.0);
  glDrawPixels(W,H,GL_RGB,GL_UNSIGNED_BYTE,fb);
  free(fb);
}

/*****************************************************************************/
/* Store color and vertex information recieved from the main program */

/* Take a new color and stores it in global variables */
void setColor3(GLubyte newr, GLubyte newg, GLubyte newb) {
  current.red = newr;
  current.green = newg;
  current.blue = newb;
}

void setColor(color new_color){
  current.red = new_color.red;
  current.green = new_color.green;
  current.blue = new_color.blue;
}

/* Make a vertex */
vertex* newVertex(int x, int y) {
  vertex* v;
  v = (vertex*) malloc(sizeof(vertex));
  v->x = x;
  v->y = y;
  return(v);
}

/* Save a triangle vertex */
void setVertex(int x, int y) {
  if (ind < 3) {
    v[ind] = newVertex(x,y);
    v[ind]->v_color.red = current.red;
    v[ind]->v_color.green = current.green;
    v[ind]->v_color.blue = current.blue;
    ind++;
  }
}

/* Start a new triangle */
void beginTriangle() {
  ind = 0;
}

/*****************************************************************************/
/* Actual triangle drawing code */

/* Makes the data structure for the edge between bottom point
   b and top point t */ 

edge* newEdge(vertex* b, vertex* t) {
  edge* e;
  int dx,dy;

  e = (edge*) malloc(sizeof(edge));
  e->x = b->x; /* edge passes through bottom vertex */
  /* D = ax + by +c - must be zero since edge goes through bottom vertex */
  e->D = 0;    

  dx = t->x - b->x;
  dy = t->y - b->y;
  if (dy == 0) {} /* doesn't matter, edge will never be used */
  else {
    e->xSmallStep = dx/dy; /* integer divide */
    if (dx < 0) {  /* positive or negative slope? */
      e->xSmallStep --;
    }
    e->a = -dy;
    e->incDSmall = e->a * e->xSmallStep + dx; 
  }
  return(e);
}

/* Called in second-to-innermost loop: 
   Moves the x position of the edge at each row */
void yInc(edge* e) {

  e->x += e->xSmallStep;
  e->D += e->incDSmall;  /* edge moves, increasing error */
  if (e->D > 0) {        /* lots of error, take a big step */
    e->x++;              /* pixel moves a bit more */
    e->D += e->a;        /* decreasing error (a is negative) */
  }
}

void swap (int *p1x, int *p1y)
{
	int *tmp = p1y;
	*p1y = *p1x;
	*p1x = *tmp;
}

/**
 * Bresenhams line algorithm adopted for linear interpolation.  Uses only integers and
 * only addition subtraction and bit shifting.
 * Reference: http://www.codekeep.net/snippets/e39b2d9e-0843-4405-8e31-44e212ca1c45.aspx
 */
int lineBresenham(int p1x, int p2x, int searchX, int sPcolor, int ePcolor)
{
    int F, x, y;

    int dy            = ePcolor - sPcolor;  // y-increment from p1 to p2
    int dx            = p2x - p1x;  // x-increment from p1 to p2
    int dy2           = (dy << 1);  // dy << 1 == 2*dy
    int dx2           = (dx << 1);
    int dy2_minus_dx2 = dy2 - dx2;  // precompute constant for speed up
    int dy2_plus_dx2  = dy2 + dx2;

    if (dy >= 0)    // m >= 0
    {
        // Case 1: 0 <= m <= 1 (Original case)
        if (dy <= dx)
        {
            F = dy2 - dx;    // initial F

            x = p1x;
            y = sPcolor;
            while (x <= p2x)
            {
            	if (x == searchX)
            		return y;
                if (F <= 0)
                {
                    F += dy2;
                }
                else
                {
                    y++;
                    F += dy2_minus_dx2;
                }
                x++;
            }
        }
        // Case 2: 1 < m < INF (Mirror about y=x line
        // replace all dy by dx and dx by dy)
        else
        {
            F = dx2 - dy;    // initial F

            y = sPcolor;
            x = p1x;
            while (y <= ePcolor)
            {
            	if (x == searchX)
            		return y;
            	if (F <= 0)
                {
                    F += dx2;
                }
                else
                {
                    x++;
                    F -= dy2_minus_dx2;
                }
                y++;
            }
        }
    }
    else    // m < 0
    {
        // Case 3: -1 <= m < 0 (Mirror about x-axis, replace all dy by -dy)
        if (dx >= -dy)
        {
            F = -dy2 - dx;    // initial F

            x = p1x;
            y = sPcolor;
            while (x <= p2x)
            {
            	if (x == searchX)
            		return y;
            	if (F <= 0)
                {
                    F -= dy2;
                }
                else
                {
                    y--;
                    F -= dy2_plus_dx2;
                }
                x++;
            }
        }
        // Case 4: -INF < m < -1 (Mirror about x-axis and mirror
        // about y=x line, replace all dx by -dy and dy by dx)
        else
        {
            F = dx2 + dy;    // initial F

            y = sPcolor;
            x = p1x;
            while (y >= ePcolor)
            {
            	if (x == searchX)
					return y;
                if (F <= 0)
                {
                    F += dx2;
                }
                else
                {
                    x++;
                    F += dy2_plus_dx2;
                }
                y--;
            }
        }
    }
}

color* integerInt(vertex* sP, vertex* eP, int x, int y)
{
	color* res = (color*)malloc(sizeof(color));
	if (sP->x > eP->x)
	{
		vertex* tmp = eP;
		eP = sP;
		sP = tmp;
	}
	int spx = sP->x;
	int epx = eP->x;
	int spy = sP->y;
	int epy = sP->y;
	
	int dx = epx - spx;
	int dy = epy - spy;
	// If the triangle has a rise > run,
	// then interpolate on Y to get a nicer interpolation
	if (dy > dx)
	{
		// TODO make it interpolate on y instead of x
		if (sP->y > eP->y)
		{
			vertex* tmp = eP;
			eP = sP;
			sP = tmp;
		}
		int spy = sP->y;
		int epy = sP->y;
		res->red = (GLubyte)lineBresenham(spy, epy, y, sP->v_color.red, eP->v_color.red);
		res->green = (GLubyte)lineBresenham(spy, epy, y, sP->v_color.green, eP->v_color.green);
		res->blue = (GLubyte)lineBresenham(spy, epy, y, sP->v_color.blue, eP->v_color.blue);
	}
	else 
	{
		res->red = (GLubyte)lineBresenham(spx, epx, x, sP->v_color.red, eP->v_color.red);
		res->green = (GLubyte)lineBresenham(spx, epx, x, sP->v_color.green, eP->v_color.green);
		res->blue = (GLubyte)lineBresenham(spx, epx, x, sP->v_color.blue, eP->v_color.blue);
	}
	//printf("(%d, %d, %d)\n", res->red, res->green , res->blue);
	return res;
}

color* interpolate(vertex* eP, vertex* sP, int x)
{
	color* result = (color*)malloc(sizeof(color));
	GLubyte r;
	GLubyte g;
	GLubyte b;
	r = sP->v_color.red + (float)((x - sP->x) * ((float)eP->v_color.red - (float)sP->v_color.red)/((float)eP->x - (float)sP->x));
	g = sP->v_color.green + (float)((x - sP->x) * ((float)eP->v_color.green - (float)sP->v_color.green)/((float)eP->x - (float)sP->x));
	b = sP->v_color.blue + (float)((x - sP->x) * ((float)eP->v_color.blue - (float)sP->v_color.blue)/((float)eP->x - (float)sP->x));
	result->red = r;
	result->green = g;
	result->blue = b;
	printf("(%d, %d, %d)\n", r, g , b);
	return result;
}

void drawIRow(vertex* left, vertex* right, int y)
{
	int x = left->x;
	while (x < right->x)
	{
		color* c = integerInt(left, right, x, y);
		DRAWPIX(x,y, c->red, c->green, c->blue);
		x++;
	}
}


/* Innermost loop - draws one row of a triangle */
void drawRow(int left, int right, int y) {
  int x = left;

  while (x < right) {
    /* draw pixel using macro (see lowlevel.h) */
    DRAWPIX(x,y,current.red,current.green,current.blue);  
    x++;
  }
}

/* Draws a triangle with bottom vertex b, top vertex t, and point
   m to the left of line from b to t */
void drawLeftTriangle(vertex* b, vertex* t, vertex* m) {
  edge* lEdge;
  edge* rEdge;
  int y;

  /* draw rows from bottom-middle edge to bottom-top edge */
  lEdge = newEdge(b,m);
  rEdge = newEdge(b,t);

  for (y=b->y; y<m->y; y++) {
	vertex* result = (vertex*)malloc(sizeof(vertex));
	vertex* result1 = (vertex*)malloc(sizeof(vertex));
//	result->v_color = *(interpolate(m, b, lEdge->x));
//	result1->v_color = *(interpolate(t, b, rEdge->x));
	result->v_color = *(integerInt(m, b, lEdge->x, y));
	result1->v_color = *(integerInt(t, b, rEdge->x, y));
	result->y = y;
	result1->y = y;
	result->x = lEdge->x;
	result1->x = rEdge->x;
	drawIRow(result,result1,y);
	free(result);
	free(result1);
    yInc(lEdge);
    yInc(rEdge);
  }

  free(lEdge);
  /* now rows from middle-top edge to bottom-top edge */
  lEdge = newEdge(m,t);
  for (; y<=t->y;y++) {
	  vertex* result = (vertex*)malloc(sizeof(vertex));
	  vertex* result1 = (vertex*)malloc(sizeof(vertex));
//	  result->v_color = *(interpolate(t, m, lEdge->x));
//	  result1->v_color = *(interpolate(t, b, rEdge->x));
	  result->v_color = *(integerInt(t, m, lEdge->x, y));
	  result1->v_color = *(integerInt(t, b, rEdge->x, y));
	  result->y = y;
	  result1->y = y;
	  result->x = lEdge->x;
	  result1->x = rEdge->x;
	  drawIRow(result,result1,y);
	  free(result);
	  free(result1);
      yInc(lEdge);
      yInc(rEdge);
  }
  
  free(lEdge);
  free(rEdge);
}
  
/* Draws a triangle with bottom vertex b, top vertex t, and point
   m to the right of line from b to t */
void drawRightTriangle(vertex* b, vertex* t, vertex* m) {
  edge* lEdge;
  edge* rEdge;
  int y;

  /* Draw rows from bottom-top edge to bottom-middle edge */
  lEdge = newEdge(b,t);
  rEdge = newEdge(b,m);

  for (y=b->y; y<m->y; y++) {
    vertex* result = (vertex*)malloc(sizeof(vertex));
	vertex* result1 = (vertex*)malloc(sizeof(vertex));
//	result->v_color = *(interpolate(t, b, lEdge->x));
//	result1->v_color = *(interpolate(m, b, rEdge->x));
	result->v_color = *(integerInt(t, b, lEdge->x, y));
	result1->v_color = *(integerInt(m, b, rEdge->x, y));
	result->y = y;
	result1->y = y;
	result->x = lEdge->x;
	result1->x = rEdge->x;
	drawIRow(result,result1,y);
	free(result);
	free(result1);
    yInc(lEdge);
    yInc(rEdge);
  }
  free(rEdge);

  /* Draw rows from bottom-top edge to middle-top edge */
  rEdge = newEdge(m,t);
  for (; y<=t->y;y++) {
	vertex* result = (vertex*)malloc(sizeof(vertex));
	vertex* result1 = (vertex*)malloc(sizeof(vertex));
//	result->v_color = *(interpolate(t, b, lEdge->x));
//	result1->v_color = *(interpolate(t, m, rEdge->x));
	result->v_color = *(integerInt(t, b, lEdge->x, y));
	result1->v_color = *(integerInt(t, m, rEdge->x, y));
	result->y = y;
	result1->y = y;
	result->x = lEdge->x;
	result1->x = rEdge->x;
	drawIRow(result,result1,y);
	free(result);
	free(result1);
	//drawRow(lEdge->x,rEdge->x, y);
    yInc(lEdge);
    yInc(rEdge);
  }

  free(lEdge);
  free(rEdge);
}
  
/* Main triangle drawing function */
void endTriangle() {

  vertex* b; vertex* t; vertex* m;
  int flag = 0x0; /* bit flags for tests */ 
  int A,B,C; /* implcit line coefficients */
  
  /* First, figure out which vertex should be 
     b (bottom vertex), t (top vertex) and m (middle vertex) */

  if (v[0]->y < v[1]->y) flag = 0x1;
  flag <<= 1; /* shift over one bit */

  if (v[1]->y < v[2]->y) flag |= 0x1;
  flag <<= 1; 

  if (v[2]->y < v[0]->y) flag |= 0x1;

  switch (flag) {

  case 0x1: /* 001 */ 
    t = v[0]; m = v[1]; b = v[2];
    break;

  case 0x2: /* 010 */
    m = v[0]; b = v[1]; t = v[2]; 
    break;

  case 0x3: /* 011 */ 
    t = v[0]; b = v[1]; m = v[2];
    break;

  case 0x4: /* 100 */
    b = v[0]; t = v[1]; m = v[2];
    break;

  case 0x5: /* 101 */
    m = v[0]; t = v[1]; b = v[2]; 
    break;

  case 0x6: /* 110 */
    b = v[0]; m = v[1]; t = v[2]; 
    break;

  default: 
    printf("Bad triangle! \n"); /* only gets here if all vertices equal */
    return;
  }

  /* Now test to see if m is right or left of line bt */
  /* Use implicit line equation */
  A = b->y  -  t->y;
  B = t->x  -  b->x;
  C = b->x * t->y  -  t->x * b->y;

  if (A*m->x + B*m->y + C > 0) 
    drawLeftTriangle(b,t,m);
  else 
    drawRightTriangle(b,t,m);

  free(v[0]);
  free(v[1]);
  free(v[2]);
}



 
