/*
 ***************************************************************************
 * \brief   Embedded Linux Module Project Work - Color Sensor
 *	    	Read colors from color sensor TCS3414 and display them in a
 *	    	console and also on the display with framebuffer technology.
 * \file    Farbsensor.c
 * \version 1.0
 * \date    28.10.2013
 * \author  Cyril Stoller
 *
 * \remark  Last Modifications:
 * 			27.12.2013 comments added
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

#include <ncurses.h>

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
	printf("%c[2J", 27);	// clear entire screen
	printf("%c[f", 27);		// move cursor to upper left of screen ("home")
	printf("\nExit via Ctrl-C\n");
	i2c_close();

	/* Terminate program */
	exit(signum);
}

/*
 *
 * Print RGB Values on the console
 *
 */

void print_rgb(UINT16 red, UINT16 green, UINT16 blue, UINT16 clear) {
	static int max = 1;
	int i;

	/* find max color value to display all values relative to that */
	if (red > max)
		max = red;
	if (green > max)
		max = green;
	if (blue > max)
		max = blue;
	if (clear > max)
		max = clear;

	/* print header*/
	printf("%c[2J", 27);	// clear entire screen
	printf("%c[f", 27);		// move cursor to upper left of screen ("home")

	printf("%c[1m", 27);	// bold on
	printf("Color values from sensor TCS3414\n");
	printf(
			"-----------------------------------------------------------------------MAX: %d4\n", max);

	/* Print RED color */
	printf("%c[1m", 27);	// bold on
	printf("RED  :");
	printf("%c[0m", 27);	// thin on
	for (i = 0; i < (int)((80.0 - 7.0) * red / max + 0.5); i++)
		printf("#");
	printf("|\n");

	/* Print GREEN color */
	printf("%c[1m", 27);	// bold on
	printf("GREEN:");
	printf("%c[0m", 27);	// thin on
	for (i = 0; i < (int)((80.0 - 7.0) * green / max + 0.5); i++)
		printf("#");
	printf("|\n");

	/* Print BLUE color */
	printf("%c[1m", 27);	// bold on
	printf("BLUE :");
	printf("%c[0m", 27);	// thin on
	for (i = 0; i < (int)((80.0 - 7.0) * blue / max + 0.5); i++)
		printf("#");
	printf("|\n");

	/* Print Clear (brightness) */
	printf("%c[1m", 27);	// bold on
	printf("BRIGHTNESS:");
	printf("%c[0m", 27);	// thin on
	for (i = 0; i < (int)((80.0 - 12.0) * clear / max + 0.5); i++)
		printf("-");
	printf("|\n");
}

/*
 ******************************************************************************
 * main
 ******************************************************************************
 */
int main(int argc, char *argv[]) {
	UINT16 green, red, blue, clear;

	/* Register signal and signal handler */
	signal(SIGINT, signal_callback_handler);

	// Open the Linux i2c device
	i2c_open();

	// Set the I2C slave address for all subsequent I2C device transfers
	i2c_set_address(TCS3414_I2C_ADDR);

	// Configure the TCS3414 color sensor
	TCS3414_Init();

	/* sleep to let user capture the init message texts */
	sleep(2);


	while (1) {
		//TCS3414_ReadColors(&green, &red, &blue, &clear);
		TCS3414_ReadColor(GREEN, &green);
		TCS3414_ReadColor(RED, &red);
		TCS3414_ReadColor(BLUE, &blue);
		TCS3414_ReadColor(CLEAR, &clear);
		//printf("Red:%3d\tGreen:%3d\tBlue:%3d\tClear:%3d\n", red, green, blue, clear);
		print_rgb(red, green, blue, clear);
		usleep(100000);

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
