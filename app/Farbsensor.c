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
#include <signal.h>

#include "TCS3414.h"

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

typedef float FLOAT32;
typedef double FLOAT64;

struct RGB_COLOR {
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

INT16 *pfb16;
INT16 x, y;

UINT32 fbfd;
UINT32 screensize;

/* Function prototype */
void signal_callback_handler(int signum);

/*
 ***************************************************************************
 * Define the function to be called when ctrl-c (SIGINT)
 * signal is sent to process
 ***************************************************************************
 */

void signal_callback_handler(int signum) {
	printf("\nExit via Ctrl-C\n");
	i2c_close();

	/* Terminate program */
	exit(signum);
}

/*
 ******************************************************************************
 * main
 ******************************************************************************
 */
int main(int argc, char *argv[]) {
	INT32 fbfd = 0;
	INT32 screensize = 0;
	UINT16 green, red, blue, clear;

	/* Register signal and signal handler */
	signal(SIGINT, signal_callback_handler);

	// Open the Linux i2c device
	i2c_open();

	// Set the I2C slave address for all subsequent I2C device transfers
	i2c_set_address(TCS3414_I2C_ADDR);

	// Configure the TCS3414 color sensor
	TCS3414_Init();

	while (1) {
		TCS3414_ReadColors(&green, &red, &blue, &clear);
		printf("Red: %5d Green: %5d Blue: %5d Clear: %5d\n", red, green, blue, clear);
		//TCS3414_ReadColor(&clear);
		//printf("brightness: %d\n", clear);
		usleep(1000000);
	}

	/*

	 // Open device file for reading and writing
	 fbfd = open("/dev/fb0", O_RDWR);
	 if (fbfd == -1) {
	 perror("Error: cannot open framebuffer device");
	 exit(errno);
	 }
	 printf("The framebuffer device fb0 was opened successfully.\n");

	 // Get variable screen information
	 if (ioctl(fbfd, FBIOGET_VSCREENINFO, &fbVarScreenInfo) == -1) {
	 perror("Error reading variable information");
	 close(fbfd);
	 exit(errno);
	 }

	 // Figure out the size of the screen in bytes
	 screensize = (fbVarScreenInfo.xres * fbVarScreenInfo.yres
	 * fbVarScreenInfo.bits_per_pixel) / 8;

	 // Map the frame buffer device memory to user space.
	 // Starting address in user space is pfb16.
	 if (fbVarScreenInfo.bits_per_pixel == BPP16) {
	 pfb16 = (INT16*) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
	 fbfd, 0);
	 if (pfb16 == (INT16*) -1) {
	 perror("Error: failed to map 16-BPP framebuffer device to memory");
	 exit(errno);
	 }

	 // Fill the screen with 16 bpp, do it for all [x,y] pixel with desired color
	 for (y = 0; y < fbVarScreenInfo.yres; y++) {
	 for (x = 0; x < fbVarScreenInfo.xres; x++) {
	 pfb16[x + y * fbVarScreenInfo.xres] = BLUE_16BPP;
	 }
	 }

	 // Cleanup
	 munmap(pfb16, screensize);
	 close(fbfd);
	 }

	 */

	printf("\n");
	i2c_close();

	return 0;
}

