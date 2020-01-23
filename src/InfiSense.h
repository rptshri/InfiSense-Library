/*
	InfiSense.h
	Bosch BMP180 pressure sensor library for the Arduino microcontroller
	Mike Grusin, SparkFun Electronics

	Uses floating-point equations from the Weather Station Data Logger project
	http://wmrx00.sourceforge.net/
	http://wmrx00.sourceforge.net/Arduino/BMP085-Calcs.pdf

	Forked from BMP085 library by M.Grusin

	version 1.0 2013/09/20 initial version
	Verison 1.1.2 - Updated for Arduino 1.6.4 5/2015
	
	Our example code uses the "beerware" license. You can do anything
	you like with this code. No really, anything. If you find it useful,
	buy me a (root) beer someday.
*/

#ifndef InfiSense_h
#define InfiSense_h

// #if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
// #else
// #include "WProgram.h"
// #endif

class InfiSense
{
public:
	InfiSense();

	void begin(boolean DEBUGenable);

	int readSoilSensor();

private:
	boolean DEBUG;
};

#endif