/* 
lowlevel.h - constants, macros, data structures 
and function defs for low-level graphics system.
*/

/* constants */

#define Roff 0    /* offset to red byte of pixel, in bytes */
#define Goff 1    /* offset to green byte of pixel, in bytes */
#define Boff 2    /* offset to blue byte of pixel, in bytes */

/* macros for pixel drawing, to avoid subroutine calls */

#define FB(x,y,c) fb[3*W*(y)+3*(x)+(c)]
#define DRAWPIX(x,y,r,g,b); FB((x),(y),Roff)=r; FB((x),(y),Goff)=g; FB((x),(y),Boff)=b;

/* data structures */

typedef struct color {
  GLubyte red;
  GLubyte green;
  GLubyte blue;
} color;

typedef struct vertex {
  int x;
  int y;
  color v_color;
} vertex;

typedef struct edge {
  int x; /* pixel x-position to right of actual edge */
  int xSmallStep; /* step size for small steps */
  int D;  /* value of ax + by + c */
  int a;  /* first coeeficient in ax + by + c */
  int incDSmall; /* how much to D changes on a small step */
} edge;

/* definitions of public functions */

void initFakeFrameBuffer(int, int);
void outputFrame(void);
void clearCanvas(void);
void setColor3(GLubyte, GLubyte, GLubyte);
void setColor(color );
void setVertex(int x, int y);
void beginTriangle(void);
void endTriangle(void);

