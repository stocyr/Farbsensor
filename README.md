Farbsensor
==========

A small xynth-software in C for the BlackBeagleBoard and the BFH-Cape. Connects to the I2C color sensor TCS3414 and imitades the measured color on the LCD screen.


## Platform

This application was developed for the Bagle Bone Black microcontroller system running a Linux operating system.
It also assumes the BBB-BFH-Cape to be mounted on top of the Beagle Bone Black.


## Purpose

The purpose of this application is to connect to the I2C color sensor TCS3414 and read the color. Then display that on the console and also on the LCD of the BBB-BFH-Cape.


## Installation

The executable `Farbsensor` has to be placed in the targets memory and executed there.

-----------------------

## Execution

After execution, the application automatically connects to the I2C driver on bus 0 and connects to the sensor on address 0x39. Then it first configures the sensor to start measuring and then reads all four color channels every 100ms. The four color channels are: *Green*, *Red*, *Blue* and *Clear*. Clear means, no color filter is applied, thus only the brighness is measured.

Those colors are then equalized (using empirically found calibration values):

- *Green* is divided through 1.6
- *Red* is devided through 1.97
- *Blue* and *Clear* are left as they are

The three color channels are then displayed on the console using a horizontal bar-style diagram which automatically adapts the dynamic on the maximal measured value.

The three color channels are also used to set the color of the display in RGB-mode. The display is painted using frame buffer technology.
