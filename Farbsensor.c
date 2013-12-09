/*
 ***************************************************************************
 * \brief   Embedded Linux Framebuffer Exercise 1.1
 *	    Draw some shapes on the Linux framebuffer.
 *	    Not optimized and only a minimal error handling is implemented.
 * \file    appFramefufferShapes.c
 * \version 1.0
 * \date    28.10.2013
 * \author  Martin Aebersold
 *
 * \remark  Last Modifications:
 * \remark  V1.0, AOM1, 28.10.2013
 ***************************************************************************
 */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <fcntl.h>

#include <math.h>

#include <linux/fb.h>

#include <sys/mman.h>
#include <sys/ioctl.h>

#include <stdbool.h>
#include <errno.h>

/*
 ***************************************************************************
 * Define some data types
 ***************************************************************************
 */

typedef signed char INT8;
typedef unsigned char UINT8;

typedef signed short INT16;
typedef unsigned short UINT16;

typedef signed int INT32;
typedef unsigned int UINT32;

typedef float	FLOAT32;
typedef double  FLOAT64;

struct RGB_COLOR
 {
  UINT8 r;
  UINT8 g;
  UINT8 b;
 };

/************************************************************************/
/* Macros and Constants							*/
/************************************************************************/

#define CONVERT_RGB24_16BPP(red, green, blue) \
	    (((red>>3)<<11) | ((green>>2)<<5) | (blue>>3))

/*
 * Color definitions for 16-BPP	  	      R    G    B
 */

#define BLACK_16BPP	CONVERT_RGB24_16BPP(  0,   0,   0)
#define RED_16BPP	CONVERT_RGB24_16BPP(255,   0,   0)
#define GREEN_16BPP	CONVERT_RGB24_16BPP(  0, 255,   0)
#define YELLOW_16BPP	CONVERT_RGB24_16BPP(255, 255,   0)
#define BLUE_16BPP	CONVERT_RGB24_16BPP(  0,   0, 255)
#define MAGENTA_16BPP	CONVERT_RGB24_16BPP(255,   0, 255)
#define CYAN_16BPP	CONVERT_RGB24_16BPP(  0, 255, 255)
#define GREY_16BPP	CONVERT_RGB24_16BPP(192, 192, 192)
#define WHITE_16BPP	CONVERT_RGB24_16BPP(255, 255, 255)

#define	BPP16		16

/************************************************************************/
/* VARS									*/
/************************************************************************/

/* Screen Info */
struct fb_var_screeninfo fbVarScreenInfo;

INT16  *pfb16;
INT16  x,y;

UINT32 fbfd;
UINT32 screensize;

/************************************************************************/
/* Swap two values							*/
/************************************************************************/

void swap(INT16* a, INT16* b)
 {
  INT16 t = *a;

  *a = *b;
  *b = t;
 }

/************************************************************************/
/* Draw a line								*/
/************************************************************************/

void Draw_Line(INT16 x1, INT16 y1, INT16 x2, INT16 y2, UINT16 color)
 {
  INT16	deltaX, deltaY, delta;
  INT16 inc_y, inc_e, inc_ne;
  INT16 slopegt1 = 0;

  /* Get the deltas */
  deltaX = abs(x1 - x2);
  deltaY = abs(y1 - y2);

  /* Swap coordinates */
  if (deltaY>deltaX)
   {
    swap(&x1, &y1);
    swap(&x2, &y2);
    swap(&deltaX, &deltaY);
    slopegt1 = 1;
   }

  /* Swap x1,x2 and y1,y2 */
  if (x1>x2)
   {
    swap(&x1, &x2);
    swap(&y1, &y2);
   }

  /* Set inc-y to pos or neg */
  if (y1 > y2)
   inc_y = -1;
  else
   inc_y = 1;

  /* Get start values */
  delta = 2 * deltaY - deltaX;
  inc_e = 2 * deltaY;
  inc_ne = 2 * (deltaY - deltaX);

  /* Draw the line */
  while (x1 < x2)
   {
    if (delta <= 0)
     delta += inc_e;
    else
     {
      delta += inc_ne;
      y1 += inc_y;
     }
    x1++;

    /* Check slope */
    if (slopegt1)
     pfb16[y1 + x1 * fbVarScreenInfo.yres] = color;
    else
     pfb16[x1 + y1 * fbVarScreenInfo.xres] = color;
   }
 }

/************************************************************************/
/* Draw a circle							*/
/************************************************************************/

void Draw_Circle(INT16 x, INT16 y, INT16 ofsX, INT16 ofsY, FLOAT32 radius, bool filled, UINT16 color)
 {
  FLOAT32 angle, stepsize;

  /* Setup the values to draw the circle */
  angle = 0.0;
  stepsize = 0.01;

  /* If we have to fill the circle, continue until the radius is zero */
  while (radius > 0)
   {
    /* Go through all angles from 0 to 2 * PI radians */
    while (angle < 2 * M_PI)
     {
      /* Calculate x, y from a vector with known radius and angle */
      x = (INT16) radius * cos (angle);
      y = (INT16) radius * sin (angle);

      /* Add screen x,y offset	*/
      x = x + ofsX;
      y = y + ofsY;

      /* Draw the pixels at location x,y with desired color*/
      pfb16[x + y * fbVarScreenInfo.xres] = color;

      /* Adjust angle */
      angle = angle + stepsize;
     }
    /* Do we have to fill the circle? */
    if (filled == true)
    {
     angle = 0.0;
     radius = radius - 1;
    }
   else
    break;
  }
 }

/************************************************************************/
/* Draw an ellipse							*/
/************************************************************************/

void Draw_Ellipse(INT16 x, INT16 y, INT16 ofsX, INT16 ofsY, FLOAT32 r1, FLOAT32 r2, bool filled, UINT16 color)
 {
  FLOAT32 angle, stepsize;

  /* Setup the values to draw the circle */
  angle = 0.0;
  stepsize = 0.01;

  /* If we have to fill the ellipse, continue until both radii are zero */
  while ((r1 > 0) && (r2 > 0))
   {

    /* Go through all angles from 0 to 2 * PI radians */
    while (angle < 2 * M_PI)
     {
      /* Calculate x, y from a vector with known radius and angle */
      x = (INT16) r1 * cos (angle);
      y = (INT16) r2 * sin (angle);

      /* Add screen x,y offset	*/
      x = x + ofsX;
      y = y + ofsY;

      /* Draw the pixels at location x,y with desired color*/
      pfb16[x + y * fbVarScreenInfo.xres] = color;

      /* Adjust angle */
      angle = angle + stepsize;
     }
    /* Do we have to fill the ellipse? */
    if (filled == true)
     {
      angle = 0.0;
      r1 = r1 - 1;
      r2 = r2 - 1;
     }
   else
    break;
  }
 }

/*
 ******************************************************************************
 * main
 ******************************************************************************
 */

int main(int argc, char *argv[])
 {
  INT32 fbfd = 0;
  INT32 screensize = 0;

  /*
   * Open device file for reading and writing
   */
  fbfd = open("/dev/fb0", O_RDWR);
  if (fbfd == -1)
   {
    perror("Error: cannot open framebuffer device");
    exit(errno);
   }
  printf("The framebuffer device fb0 was opened successfully.\n");

  /*
   * Get variable screen information
   */
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &fbVarScreenInfo) == -1)
   {
    perror("Error reading variable information");
    close(fbfd);
    exit(errno);
   }

  /* Figure out the size of the screen in bytes */
  screensize = (fbVarScreenInfo.xres * fbVarScreenInfo.yres * fbVarScreenInfo.bits_per_pixel) / 8;

  /*
   * Map the frame buffer device memory to user space.
   * Starting address in user space is pfb16.
   */
   if (fbVarScreenInfo.bits_per_pixel == BPP16)
    {
     pfb16 = (INT16*) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
     if (pfb16 == (INT16*) -1)
      {
       perror("Error: failed to map 16-BPP framebuffer device to memory");
       exit(errno);
      }

     /* Fill the screen with 16 bpp, do it for all [x,y] pixel with desired color	*/
     for (y=0; y<fbVarScreenInfo.yres; y++)
      {
       for (x=0; x<fbVarScreenInfo.xres; x++)
        {
         pfb16[x + y * fbVarScreenInfo.xres]  = BLUE_16BPP;
        }
      }

     /* Draw a line from the corner left,top to right,bottom */
     Draw_Line(0, 0, 800, 480, WHITE_16BPP);

     /* Draw a line from the corner left,bottom to right,top */
     Draw_Line(0, 480, 800, 0, WHITE_16BPP);

     /* Draw a filled circle, background color is yellow */
     Draw_Circle(0, 0, fbVarScreenInfo.xres / 2, fbVarScreenInfo.yres / 2, 50.0, true, YELLOW_16BPP);

     /* Draw a seond filled circle, background color is red */
     Draw_Circle(0, 0, fbVarScreenInfo.xres / 2, fbVarScreenInfo.yres / 2, 25.0, true, RED_16BPP);

     /* Draw a filled ellipse, background color is green */
     Draw_Ellipse(0, 0, fbVarScreenInfo.xres / 4, fbVarScreenInfo.yres / 4, 30.0, 50.0, true, GREEN_16BPP);

     /* Cleanup */
     munmap(pfb16, screensize);
     close(fbfd);
   }
  return 0;
 }

