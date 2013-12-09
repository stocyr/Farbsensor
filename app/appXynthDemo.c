/*
 ******************************************************************************
 * \brief   Embedded Linux Xynth Example
 *	    Graphical control of the LED-1
 *	    Only a minimal error handling is implemented.
 * \file    appXynthLedOnOff.c
 * \version 1.0
 * \date    28.10.2013
 * \author  Martin Aebersold
 *
 * \remark  Last Modifications:
 * \remark  V1.0, AOM1, 28.10.2013
 ******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include <linux/input.h>

#include <sys/types.h>
#include <sys/stat.h>


#include "xynth_.h"

/* Define some useful constants */
#define	GPIO_SYSFS_EXPORT	0
#define GPIO_SYSFS_DIRECTION	1
#define GPIO_SYSFS_VALUE	2
#define	GPIO_SYSFS_UNEXPORT	3

/* Function prototype */
void signal_callback_handler(int signum);

/* Variables */
typedef struct img_data_s
 {
  int imgf;       /* full screen */
  int imgp;       /* image pos   */
  int imgs;       /* scale image */
  s_list_t *imgl; /* images list */
 } img_data_t;

int gpio_fd;

/*
 ***************************************************************************
 * Syfs gpio handler
 ***************************************************************************
 */

int sysfs_gpio_handler(char str[], char gpio[], char function)
 {
  switch (function)
   {
    /* Open gpio export file descriptor */
    case GPIO_SYSFS_EXPORT:
     gpio_fd = open(str, O_WRONLY);
     if (gpio_fd  < 0)
      {
       printf("Cannot open GPIO to export it\n");
       return -1;
      }
     /* Write the gpio you want to export and close the export interface. */
     write(gpio_fd , gpio, strlen(gpio)+1);
     close(gpio_fd);
    break;

    /* Open gpio direction file descriptor */
    case GPIO_SYSFS_DIRECTION:
     gpio_fd = open(str, O_RDWR);
     if (gpio_fd  < 0)
      {
       printf("Cannot open GPIO direction\n");
       return -1;
      }
     /* Write the gpio direction and close the direction interface. */
     write(gpio_fd, gpio, strlen(gpio)+1);
     close(gpio_fd);
    break;

    /* Open gpio value file descriptor */
    case GPIO_SYSFS_VALUE:
     gpio_fd = open(str, O_RDWR);
     if (gpio_fd < 0)
      {
       printf("Cannot open GPIO value for %s\n", str);
       return -1;
      }
     /* Write the gpio vale and close the value interface. */
     write(gpio_fd, gpio, strlen(gpio)+1);
     close(gpio_fd);
    break;

    /* Open gpio unexport file descriptor */
    case GPIO_SYSFS_UNEXPORT:
     gpio_fd = open(str, O_WRONLY);
     if (gpio_fd  < 0)
      {
       printf("Cannot open GPIO to unexport it\n");
       return -1;
      }
     /* Write the gpio you want to unexport and close the export interface. */
     write(gpio_fd , gpio, strlen(gpio)+1);
     close(gpio_fd);
     break;
   }
  return 0;
 }

/*
 ***************************************************************************
 * Define the function to be called when ctrl-c (SIGINT)
 * signal is sent to process
 ***************************************************************************
 */

void signal_callback_handler(int signum)
 {
  printf("\nExit via Ctrl-C\n");

  /* Turn the LEDs off */
  sysfs_gpio_handler("/sys/class/gpio/gpio67/value", "1", GPIO_SYSFS_VALUE);
  sysfs_gpio_handler("/sys/class/gpio/gpio68/value", "1", GPIO_SYSFS_VALUE);
  sysfs_gpio_handler("/sys/class/gpio/gpio61/value", "1", GPIO_SYSFS_VALUE);

  /* Terminate program */
  exit(signum);
 }

/*
 ******************************************************************************
 * Toggle the LEDs
 ******************************************************************************
 */

static void led1_toggle(s_window_t *window, s_event_t *event, s_handler_t *handler)
 {
	static bool on = 0;
  /* toggle the LED-1 */
	if(on == true)
		sysfs_gpio_handler("/sys/class/gpio/gpio67/value", "0", GPIO_SYSFS_VALUE);
	else
		sysfs_gpio_handler("/sys/class/gpio/gpio67/value", "1", GPIO_SYSFS_VALUE);
	on = !on;

  s_fillbox(window->surface, 158, 200, 70, 70, s_rgbcolor(window->surface, 255, 255, 255));
  s_fillbox(window->surface, 162, 204, 62, 62, s_rgbcolor(window->surface, 30, 255, 30));
  usleep(40000);

  s_fillbox(window->surface, 158, 200, 70, 70, s_rgbcolor(window->surface, 0, 0, 0));
  s_fillbox(window->surface, 162, 204, 62, 62, s_rgbcolor(window->surface, 30, 255, 30));
  usleep(40000);
 }

static void led2_toggle(s_window_t *window, s_event_t *event, s_handler_t *handler)
 {
	static bool on = 0;
  /* toggle the LED-2 */
	if(on == true)
		sysfs_gpio_handler("/sys/class/gpio/gpio68/value", "0", GPIO_SYSFS_VALUE);
	else
		sysfs_gpio_handler("/sys/class/gpio/gpio68/value", "1", GPIO_SYSFS_VALUE);
	on = !on;

  s_fillbox(window->surface, 380, 200, 70, 70, s_rgbcolor(window->surface, 255, 255, 255));
  s_fillbox(window->surface, 384, 204, 62, 62, s_rgbcolor(window->surface, 30, 255, 30));
  usleep(40000);

  s_fillbox(window->surface, 380, 200, 70, 70, s_rgbcolor(window->surface, 0, 0, 0));
  s_fillbox(window->surface, 384, 204, 62, 62, s_rgbcolor(window->surface, 30, 255, 30));
  usleep(40000);
 }

/*
 ******************************************************************************
 * Handler for timer0
 ******************************************************************************
 */

static void timer0_handler(s_window_t *window, s_timer_t *timer)
 {
  static bool toggleFlag = 0;

  if (toggleFlag == true)
   /* Turn the LED-4 on */
   sysfs_gpio_handler("/sys/class/gpio/gpio61/value", "0", GPIO_SYSFS_VALUE);
  else
   /* Turn the LED-4 off */
   sysfs_gpio_handler("/sys/class/gpio/gpio61/value", "1", GPIO_SYSFS_VALUE);
 toggleFlag = !toggleFlag;
 }

/*
 ******************************************************************************
 * main
 ******************************************************************************
 */

int main (int argc, char *argv[])
 {
  int x;
  int y;
  int w;
  int h;

  s_font_t *font;
  s_handler_t *hndl;
  s_window_t *window;
  s_timer_t *timer;
  s_image_t *img1;
  s_image_t *img2;
  char *file1 = "tux_80x96.png";
  char *file2 = "xkcd.png";

  /* Register signal and signal handler */
  signal(SIGINT, signal_callback_handler);

  /* Setup gpio sysfs for LED1 */
  sysfs_gpio_handler("/sys/class/gpio/export", "67", GPIO_SYSFS_EXPORT);
  sysfs_gpio_handler("/sys/class/gpio/gpio67/direction", "out", GPIO_SYSFS_DIRECTION);
  sysfs_gpio_handler("/sys/class/gpio/gpio67/value", "1", GPIO_SYSFS_VALUE);

  /* Setup gpio sysfs for LED2 */
  sysfs_gpio_handler("/sys/class/gpio/export", "68", GPIO_SYSFS_EXPORT);
  sysfs_gpio_handler("/sys/class/gpio/gpio68/direction", "out", GPIO_SYSFS_DIRECTION);
  sysfs_gpio_handler("/sys/class/gpio/gpio68/value", "1", GPIO_SYSFS_VALUE);

  /* Setup gpio sysfs for LED4 */
  sysfs_gpio_handler("/sys/class/gpio/export", "61", GPIO_SYSFS_EXPORT);
  sysfs_gpio_handler("/sys/class/gpio/gpio61/direction", "out", GPIO_SYSFS_DIRECTION);
  sysfs_gpio_handler("/sys/class/gpio/gpio61/value", "1", GPIO_SYSFS_VALUE);

  /* Init window and create it	*/
  s_window_init(&window);
  s_window_new(window, WINDOW_TYPE_MAIN | WINDOW_TYPE_NOFORM | WINDOW_TYPE_DESKTOP, NULL);
  s_window_set_cursor(window, CURSOR_TYPE_NONE);
  s_window_set_resizeable(window, 0);
  s_window_set_alwaysontop(window, 1);

  /* Set width and height */
  w = 600;
  h = 400;
  x = (window->surface->width - w) / 2;
  y = (window->surface->height - h) / 2;
  s_window_set_coor(window, WINDOW_TYPE_NOFORM, x, y, w, h);
  s_free(window->surface->vbuf);
  window->surface->width = w;
  window->surface->height = h;
  window->surface->vbuf = (unsigned char *) malloc(w * h * window->surface->bytesperpixel);

  /* Draw window with a gradient color */
  s_fillbox(window->surface, 0, 0, w, h, s_rgbcolor(window->surface, 0, 0, 0));
  for (y=2; y<h-2; y++)
   {
    s_fillbox(window->surface, 2, y, w - 4, 1, s_rgbcolor(window->surface, 255 - (int) ((float) y * 255.00 / 400.00),
                                                                           255 - (int) ((float) y * 255.00 / 400.00),
		                                                           255));
   }

  /* Initialize img structs */
  s_image_init(&img1);
  s_image_img(file1, img1);
  s_image_init(&img2);
  s_image_img(file2, img2);

  /* Paste imgs to the surface */
  s_putboxrgba(window->surface, 30 + img1->x, 20 + img1->y, img1->w, img1->h, img1->rgba);
  s_putboxrgba(window->surface, 40 + img2->x, 170 + img2->y, img2->w, img2->h, img2->rgba);

  /* Uninitialize img struct */
  s_image_uninit(img1);
  s_image_uninit(img2);

  /* Setup the fonts for the heading text */
  s_font_init(&font, "arial.ttf");
  s_font_set_str(font, "Xynth LED-Demo");
  s_font_set_size(font, 32);
  s_font_get_glyph(font);
  s_image_get_handler(font->glyph.img);
  s_putboxrgba(window->surface, 165, 80, font->glyph.img->w, font->glyph.img->h, font->glyph.img->rgba);
  s_fillbox(window->surface, 165, 80 + font->glyph.img->h + 2, font->glyph.img->w, 2, 0);
  s_font_uninit(font);

  /* Create the first button */
  s_fillbox(window->surface, 158, 200, 70, 70, s_rgbcolor(window->surface, 0, 0, 0));
  s_fillbox(window->surface, 162, 204, 62, 62, s_rgbcolor(window->surface, 30, 255, 30));

  /* Setup the fonts for the led off text */
  s_font_init(&font, "arial.ttf");
  s_font_set_str(font, "LED 1");
  s_font_set_size(font, 20);
  s_font_set_rgb(font, 255, 255, 0);
  s_font_get_glyph(font);
  s_image_get_handler(font->glyph.img);
  s_putboxrgba(window->surface, 166, 180, font->glyph.img->w, font->glyph.img->h, font->glyph.img->rgba);
  s_font_uninit(font);
	
  /* Install the mouse callback handler for the led_off button */
  s_handler_init(&hndl);
  hndl->type = MOUSE_HANDLER;
  hndl->mouse.x = 160;
  hndl->mouse.y = 200;
  hndl->mouse.w = 100;
  hndl->mouse.h = 100;
  hndl->mouse.o = led1_toggle;
  hndl->mouse.button = MOUSE_BUTTON_LEFT;
  s_handler_add(window, hndl);

  /* Create the second button */
  s_fillbox(window->surface, 380, 200, 70, 70, s_rgbcolor(window->surface, 0, 0, 0));
  s_fillbox(window->surface, 384, 204, 62, 62, s_rgbcolor(window->surface, 30, 255, 30));

  /* Setup the fonts for the led_on text */
  s_font_init(&font, "arial.ttf");
  s_font_set_str(font, "LED 2");
  s_font_set_size(font, 20);
  s_font_set_rgb(font, 255, 255, 0);
  s_font_get_glyph(font);
  s_image_get_handler(font->glyph.img);
  s_putboxrgba(window->surface, 390, 180, font->glyph.img->w, font->glyph.img->h, font->glyph.img->rgba);
  s_font_uninit(font);

  /* Install the mouse callback handler for the led_on button */
  s_handler_init(&hndl);
  hndl->type = MOUSE_HANDLER;
  hndl->mouse.x = 380;
  hndl->mouse.y = 200;
  hndl->mouse.w = 100;
  hndl->mouse.h = 100;
  hndl->mouse.o = led2_toggle;
  hndl->mouse.button = MOUSE_BUTTON_LEFT;
  s_handler_add(window, hndl);

  /* Setup a timer handler */
  s_timer_init(&timer);
  timer->timeval = 500;
  timer->cb = timer0_handler;
  s_timer_add(window, timer);

  /* Show the window on the screen */
  s_window_show(window);
  s_window_main(window);

  return 0;
 }

