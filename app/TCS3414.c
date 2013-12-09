/*
 ***************************************************************************
 * \brief   Embedded Linux I2C Exercise 4.3
 *	    	Basic i2c communication with the TCS3414 Color Sensor
 *          on the BBB-BFH-Cape. Show the current color on the console.
 *	    	Only a minimal error handling is implemented.
 * \file    appMCP9802.c
 * \version 1.0
 * \date    28.11.2013
 * \author  Martin Aebersold
 *
 * \remark  Last Modifications:
 * \remark  V1.0, AOM1, 28.11.2013
 ***************************************************************************
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

/* TCS3414 Register pointers */
#define TCS3414_CONTROL		0x00	/* Control Register */
#define TCS3414_TIMING		0x01	/* Integration Time/Gain Register */
#define TCS3414_GAIN		0x07	/* ADC Gain Register */
#define TCS3414_DATA1LOW	0x10	/* Green low Register */
#define TCS3414_DATA1HIGH	0x11	/* Green high Register */
#define TCS3414_DATA1LOW	0x12	/* Red low Register */
#define TCS3414_DATA1HIGH	0x13	/* Red high Register */
#define TCS3414_DATA1LOW	0x14	/* Blue low Register */
#define TCS3414_DATA1HIGH	0x15	/* Blue high Register */
#define TCS3414_DATA1LOW	0x16	/* Clear low Register */
#define TCS3414_DATA1HIGH	0x17	/* Clear high Register */

/* i2c Address of TCS3414 device */
#define TCS3414_I2C_ADDR 	0x39

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

/*
 ***************************************************************************
 * Vars
 ***************************************************************************
 */

/* File descriptor to i2cdev */
INT32 i2c_fd;

/* Configuration register */
UINT8 tcs3414_config_register;

/* i2c communication buffer */
UINT8 i2cCommBuffer[8];

/************************************************************************/
/* Open the i2c interface						*/
/************************************************************************/

INT16 i2c_open(void)
 {
 /* Open the Linux i2c device */
 i2c_fd = open("/dev/i2c-1", O_RDWR);
  if (i2c_fd < 0)
   {
    perror("i2cOpen");
    return -1;
   }
  return 0;
 }

/************************************************************************/
/* Close the i2c interface						*/
/************************************************************************/

void i2c_close(void)
 {
  close(i2c_fd);
 }

/************************************************************************/
/* Set i2c address							*/
/************************************************************************/

INT16 i2c_set_address(UINT8 i2cAddress)
 {
  /* Set the I2C slave address for all subsequent I2C device transfers */
  if (ioctl(i2c_fd, I2C_SLAVE, i2cAddress) < 0)
   {
    perror("i2cSetAddress");
    return -1;
   }
  return 0;
 }

/************************************************************************/
/* Write to the i2c device						*/
/************************************************************************/

INT16 i2c_write(UINT8 *i2cBuffer, UINT16 i2cLen)
 {
  /* Write to the i2c device */
  if (write(i2c_fd, i2cBuffer, i2cLen) != i2cLen)
   {
    perror("write error!");
    return -1;
   }
  return 0;
 }

/************************************************************************/
/* Read from the i2c device						*/
/************************************************************************/

INT16 i2c_read(UINT8 *i2cBuffer, UINT16 i2cLen)
 {
  /* Read from the i2c device */
  if (read(i2c_fd, i2cCommBuffer, i2cLen) != i2cLen)
   {
    perror("read error");
    return -1;
   }
  return 0;
 }

/************************************************************************/
/* Initialize the temperature sensor MCP9802				*/
/************************************************************************/

INT16 MCP9800_Init(void)
 {
  /* Setup i2c buffer for the config register */
  i2cCommBuffer[0] = MCP9800_CONFIG;
  i2cCommBuffer[1] = MCP9800_12_BIT;

  /* Write buffer data to i2c device */
  if (i2c_write(i2cCommBuffer, 2) < 0)
   {
    perror("read error");
    return -1;
   }

  /* Setup mcp9800 register to read the temperature */
  i2cCommBuffer[0] = MCP9800_TEMP;

  /* Write buffer data to i2c device */
  if (i2c_write(i2cCommBuffer, 1) < 0)
   {
    perror("read error");
    return -1;
   }
  return 0;
 }

/************************************************************************/
/* Get the current temperature from the MCP9802 sensor			*/
/************************************************************************/

void MCP9800_ReadTemperature(FLOAT32 *GradCelsius)
 {
  FLOAT32 TempC;
  INT16   Temperature;

  /* Setup mcp9800 register to read the temperature */
  i2cCommBuffer[0] = MCP9800_TEMP;

  /* Write data to i2c device */
  i2c_write(i2cCommBuffer, 1);

  /* Read the current temperature from the mcp9800 device */
  i2c_read(i2cCommBuffer, 2);

  /* Assemble the temperature values */
  Temperature = ((i2cCommBuffer[0] << 8) | i2cCommBuffer[1]);
  Temperature = Temperature >> 4;

  /* Convert current temperature to float */
  TempC = 1.0 * Temperature * 0.0625;
  *GradCelsius = TempC;
 }

/************************************************************************/
/* main()								*/
/************************************************************************/

int main(int argc, char** argv)
 {
  UINT16  i;
  FLOAT32 temperature;

  /* Open the Linux i2c device */
  i2c_open();

  /* Set the I2C slave address for all subsequent I2C device transfers */
  i2c_set_address(MCP9800_I2C_ADDR);

  /* Configure the MCP9802 temperature sensor */
  MCP9800_Init();

  /* Inform the user about the ambient temperature */
  for (i=0; i<15; i++)
   {
    MCP9800_ReadTemperature(&temperature);
    printf("\nGrad Celsius: %3.2f\n", temperature);
    printf("Fahrenheit:   %3.2f\n", temperature * 1.8 + 32.0);
    sleep(1);
   }

  /* Close file descriptor and exit program */
  printf("\n");
  i2c_close();
  return 0;
 }

