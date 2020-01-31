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

	void begin(boolean DEBUG);

	int readSoilSensor();

	float readTempSensor();
	float readDS18B20();

	float readLeafSensor(); ///yet to be defined

	float readUVSensor();
	float readML8511();

	int readRainSensor();

	float readBMEAirHumidity();
	float readBMEAirTemp();
	float readBMEAirPressure();
	float readBMEAltitude();

	float readLUXSensor();
	float readBH1750();

	void triggerPumpOn();
	void triggerRelayOn();
	void triggerPumpOff();
	void triggerRelayOff();

	float readBatteryVoltage();

	void triggerLedOn();
	void turnLedOn();
	void triggerLedOff();
	void turnLedOff();

	void attachButton();
	bool readButton();

private:
	float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);

	boolean _debug;
	int _TempPin;
};

typedef struct Inst
{
    int row;
    int startloc;
	char *data;
} Inst;

class LcdDisplay
{
public:
	LcdDisplay();

	void begin();

	void setScreens(int getScreens);

	void printLCD(int screennum, int rownum, int startloc1, char *data1);

private:
	
	Inst S[][10];
	int _totalScreens = 0;
};

#define ONE_WIRE_BUS D5
#define RAIN_MEASURE A0
#define SEALEVELPRESSURE_HPA (1013.25)
#define RELAY_PIN 13
#define LED_PIN 12
#define BUTTON_PIN 2

#endif