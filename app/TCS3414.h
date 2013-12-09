/*
 ***************************************************************************
 * \brief   Embedded Linux I2C Exercise 4.3
 *	    	Basic i2c communication with the TCS3414 Color Sensor
 *          on the BBB-BFH-Cape. Show the current color on the console.
 *	    	Only a minimal error handling is implemented.
 * \file    TCS3414.h
 * \version 1.0
 * \date    09.12.2013
 * \author  Cyril Stoller
 *
 * \remark  Last Modifications:
 ***************************************************************************
 */

#ifndef TCS3414_H
#define TCS3414_H

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
#define TCS3414_DATABLOCK	0x0F	/* SMBus block read (read all 8 registers at once) */
#define TCS3414_DATA1LOW	0x10	/* Green low Register */
#define TCS3414_DATA1HIGH	0x11	/* Green high Register */
#define TCS3414_DATA2LOW	0x12	/* Red low Register */
#define TCS3414_DATA2HIGH	0x13	/* Red high Register */
#define TCS3414_DATA3LOW	0x14	/* Blue low Register */
#define TCS3414_DATA3HIGH	0x15	/* Blue high Register */
#define TCS3414_DATA4LOW	0x16	/* Clear low Register */
#define TCS3414_DATA4HIGH	0x17	/* Clear high Register */

/* i2c Address of TCS3414 device */
#define TCS3414_I2C_ADDR 	0x39

/* TCS3414 CONTROL REGISTER DATA */
#define TCS3414_POWER_ON_ADC_EN	0x03

/* TCS3414 COMMAND CONTROL -> to be OR-linked with register pointer */
#define TCS3414_BYTE_WISE	0x80
#define TCS3414_BLOCK_WISE	0xC0

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

/*
 ***************************************************************************
 *  Prototypes
 ***************************************************************************
 */

extern INT16 i2c_open(void);
extern void i2c_close(void);
extern INT16 i2c_set_address(UINT8 i2cAddress);
extern INT16 i2c_write(UINT8 *i2cBuffer, UINT16 i2cLen);
extern INT16 i2c_read(UINT8 *i2cBuffer, UINT16 i2cLen);
extern INT16 TCS3414_Init(void);
extern void TCS3414_ReadColors(UINT16* green, UINT16* red, UINT16* blue,
		UINT16* clear);
extern void TCS3414_ReadColor(UINT16* value);

#endif

