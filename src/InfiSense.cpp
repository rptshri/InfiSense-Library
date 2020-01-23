/*
	InfiSense.cpp
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

#include "InfiSense.h"
#include <Wire.h>
#include <stdio.h>
#include <math.h>

#ifndef ADS1015
#define ADS1015
#include "Adafruit_ADS1015.h"
#endif

InfiSense::InfiSense()
{
}

void InfiSense::begin(boolean DEBUGenable)
{
    Serial.println();
    Serial.println("--------------------------------------------------");
    Serial.println("Precifarm - Practical Approach to Agricultural IoT");
    Serial.println("--------------------------------------------------");

    if (DEBUGenable)
    {
        DEBUG = true;
        Serial.println("-----DEBUG is ON-----");
    }
    else
    {
        DEBUG = false;
        Serial.println("-----DEBUG is OFF-----");
    }
}

int InfiSense::readSoilSensor()
{
    Adafruit_ADS1115 ads(0x48); // Addr connected to
    ads.begin();
    const int numReadings = 10;
    int readings[numReadings]; // the readings from the analog input
    int readIndex = 0;         // the index of the current reading
    int total = 0;             // the running total
    double average = 0;        // the average

    // initialize all the readings to 0:
    for (int thisReading = 0; thisReading < numReadings; thisReading++)
    {
        readings[thisReading] = 0;
    }

    for (int i = 0; i <= 30; i++)
    {
        total = total - readings[readIndex];              // subtract the last reading:
        readings[readIndex] = ads.readADC_SingleEnded(0); // read from the sensor:
        total = total + readings[readIndex];              // add the reading to the total:
        readIndex += 1;                                   // advance to the next position in the array:

        // if we're at the end of the array...
        if (readIndex >= numReadings)
        {
            // ...wrap around to the beginning:
            readIndex = 0;
        }
        average = total / numReadings; // calculate the average:
        if (DEBUG)
        {
            Serial.print(average);
            Serial.print(",");
        }
        delay(0);
    }
    int out = map(average, 0, 65536, 0, 100);
    if (DEBUG)
    {
        Serial.println("");
        Serial.print(average);
        Serial.print(",");
        Serial.println(out);
    }
    // send it to the computer as ASCII digits
    return out;
}
