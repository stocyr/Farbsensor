/*
 ***************************************************************************
 * \brief   Embedded Linux I2C Exercise 4.3
 *	    	Basic i2c communication with the TCS3414 Color Sensor
 *          on the BBB-BFH-Cape. Show the current color on the console.
 *	    	Only a minimal error handling is implemented.
 * \file    TCS3414.c
 * \version 1.0
 * \date    09.12.2013
 * \author  Cyril Stoller
 *
 * \remark  Last Modifications:
 ***************************************************************************
 */

#include "TCS3414.h"

/*
 ***************************************************************************
 * Vars
 ***************************************************************************
 */

/* File descriptor to i2cdev */
INT32 i2c_fd;

/* i2c communication buffer */
UINT8 i2cCommBuffer[8];

/************************************************************************/
/* Open the i2c interface												*/
/************************************************************************/

INT16 i2c_open(void) {
	/* Open the Linux i2c device */
	i2c_fd = open("/dev/i2c-1", O_RDWR);
	if (i2c_fd < 0) {
		perror("i2cOpen");
		return -1;
	}
	return 0;
}

/************************************************************************/
/* Close the i2c interface												*/
/************************************************************************/

void i2c_close(void) {
	close(i2c_fd);
}

/************************************************************************/
/* Set i2c address														*/
/************************************************************************/

INT16 i2c_set_address(UINT8 i2cAddress) {
	/* Set the I2C slave address for all subsequent I2C device transfers */
	if (ioctl(i2c_fd, I2C_SLAVE, i2cAddress) < 0) {
		perror("i2cSetAddress");
		return -1;
	}
	return 0;
}

/************************************************************************/
/* Write to the i2c device												*/
/************************************************************************/

INT16 i2c_write(UINT8 *i2cBuffer, UINT16 i2cLen) {
	/* Write to the i2c device */
	if (write(i2c_fd, i2cBuffer, i2cLen) != i2cLen) {
		perror("i2c write error! ");
		return -1;
	}
	return 0;
}

/************************************************************************/
/* Read from the i2c device												*/
/************************************************************************/

INT16 i2c_read(UINT8 *i2cBuffer, UINT16 i2cLen) {
	/* Read from the i2c device */
	if (read(i2c_fd, i2cBuffer, i2cLen) != i2cLen) {
		perror("i2c read error! ");
		return -1;
	}
	return 0;
}

/************************************************************************/
/* Initialize the color sensor TCS3414									*/
/************************************************************************/
INT16 TCS3414_Init(void) {
	/* Setup i2c buffer for the control register */
	i2cCommBuffer[0] = TCS3414_BYTE_WISE | TCS3414_CONTROL;

	/* Write buffer data to i2c device */
	if (i2c_write(i2cCommBuffer, 1) < 0) {
		perror("init write error");
		return -1;
	}

	/* Setup TCS3414 register to power on and enable ADC converting */
	i2cCommBuffer[0] = TCS3414_POWER_ON_ADC_EN;

	/* Write buffer data to i2c device */
	if (i2c_write(i2cCommBuffer, 1) < 0) {
		perror("init write error");
		return -1;
	}

	i2c_read(i2cCommBuffer, 1);

	printf("\nReceived after sending POWER_ON: %d (should be 3)\n\n", i2cCommBuffer[0]);

	return 0;
}

/************************************************************************
 * Get all 4 current color values from the TCS3414 sensor. Each has a
 * low and a high byte. The colors are as follows: GREEN, RED, BLUE,
 * CLEAR. CLEAR means no color filtering is applied on this sensor, thus
 * simply the birghtness is returned.
 ************************************************************************/

void TCS3414_ReadColors(UINT16* green, UINT16* red, UINT16* blue, UINT16* clear) {
	/* Setup TCS3414 register to read all 4 colors as a block */
	i2cCommBuffer[0] = TCS3414_BLOCK_WISE | TCS3414_DATABLOCK;
	i2cCommBuffer[1] = 0x08;	// page 10 of datasheet:
	/*
When an SMBus Block Write or Block Read is initiated (see description of COMMAND Register), the byte
following the COMMAND byte is ignored but is a requirement of the SMBus specification. This field contains
the byte count (i.e. the number of bytes to be transferred).
	 */

	/* Write data to i2c device */
	i2c_write(i2cCommBuffer, 2);

	/* Read the color value block (8 bytes) from the device */
	i2c_read(i2cCommBuffer, 8);



	*green = i2cCommBuffer[0] | (i2cCommBuffer[1] << 8);
	*red   = i2cCommBuffer[2] | (i2cCommBuffer[3] << 8);
	*blue  = i2cCommBuffer[4] | (i2cCommBuffer[5] << 8);
	*clear = i2cCommBuffer[6] | (i2cCommBuffer[7] << 8);
}

void TCS3414_ReadColor(UINT16* value) {
	/* Setup TCS3414 register to read "clear" LOW*/
	i2cCommBuffer[0] = TCS3414_BYTE_WISE | TCS3414_DATA4LOW;

	/* Write data to i2c device */
	i2c_write(i2cCommBuffer, 1);

	/* Read the color value block (8 bytes) from the device */
	i2c_read(i2cCommBuffer, 1);

	*value = i2cCommBuffer[0];

	/* Setup TCS3414 register to read "clear" HIGH*/
	i2cCommBuffer[0] = TCS3414_BYTE_WISE | TCS3414_DATA4HIGH;

	/* Write data to i2c device */
	i2c_write(i2cCommBuffer, 1);

	/* Read the color value block (8 bytes) from the device */
	i2c_read(i2cCommBuffer, 1);

	*value |= i2cCommBuffer[0] << 8;
}
