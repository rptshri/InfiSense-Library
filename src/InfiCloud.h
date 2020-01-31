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

#ifndef InfiCloud_h
#define InfiCloud_h

// #if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
// #else
// #include "WProgram.h"
// #endif

#ifndef DEFAULT_PROJECT_NAME
#define DEFAULT_PROJECT_NAME "First Project"
#endif

#define LEDpin 12

typedef struct Value
{
    char *id;
    // char *context;
    float value_id;
    // unsigned long timestamp;
} Value;

class InfiCloud
{
public:
    InfiCloud(char *token);

    void setDebug(bool debug);

    void add(char *variable_id, float value);
    void add(char *variable_id, float value, char *ctext);
    void add(char *variable_id, float value, unsigned long timestamp);
    void add(char *variable_id, float value, char *ctext, unsigned long timestamp);

    String makePayload();

    bool attachLEDToStatus(bool status);

    int connectToWiFi(char *_ssid, char *_pass);

    int sendToCloud(char *_host);

private:
    void idAsMac();
    void ledOn();
    void ledOff();
    void dataSentLED();
    void dataNotSentLED();

    char *_espID = (char *)malloc(sizeof(char) * 100);
    uint8_t maxValues;
    uint8_t currentValue;
    Value *val;
    char *_token;
    char *_projectName;
    bool _debug = 0;
    bool _ledForDebug = 0;
    bool _status = false;

    String all;
    String str;
};

#endif