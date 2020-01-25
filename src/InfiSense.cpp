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
#include <string.h>

#include "Adafruit_ADS1015.h"
Adafruit_ADS1115 ads(0x48); // Addr connected to

#include "OneWire.h"

#include "DallasTemperature.h"

#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

#include "BH1750.h"



InfiSense::InfiSense()
{
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    // Serial.begin(9600);
}

void InfiSense::begin(boolean DEBUG)
{

    Serial.println();
    Serial.println("--------------------------------------------------");
    Serial.println("Precifarm - Practical Approach to Agricultural IoT");
    Serial.println("--------------------------------------------------");

    if (DEBUG)
    {
        _debug = true;
        Serial.println("-----DEBUG is ON-----");
    }
    else
    {
        _debug = false;
        Serial.println("-----DEBUG is OFF-----");
    }
}

int InfiSense::readSoilSensor()
{

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
        if (_debug)
        {
            Serial.print(average);
            Serial.print(",");
        }
        delay(0);
    }
    int out = map(average, 0, 65536, 0, 100);
    if (_debug)
    {
        Serial.println("");
        Serial.print(average);
        Serial.print(",");
        Serial.println(out);
    }
    // send it to the computer as ASCII digits
    return out;
}

float InfiSense::readDS18B20()
{
    return (InfiSense::readTempSensor());
}

float InfiSense::readTempSensor()
{

    OneWire oneWire(ONE_WIRE_BUS);
    DallasTemperature DS18B20(&oneWire);
    char temperatureString[6];

    DS18B20.begin();
    float temp;

    do
    {
        DS18B20.requestTemperatures();
        temp = DS18B20.getTempCByIndex(0);
        delay(100);
    } while (temp == 85.0 || temp == (-127.0));

    float temperature = temp;

    dtostrf(temperature, 2, 2, temperatureString);
    if (_debug)
    {
        Serial.println("----------------------");
        Serial.print("SoilTemp: ");
        Serial.print(temperatureString);
        Serial.println(" 'C");
        // Serial.println("----------------------");
    }

    return temperature;
}

float InfiSense::readML8511()
{
    return (InfiSense::readUVSensor());
}

float InfiSense::readUVSensor()
{
    ads.begin();
    float UV_Raw = ads.readADC_SingleEnded(1);
    float voltage = (UV_Raw / 32768) * 6.144;
    float UVindex = mapfloat(voltage, 0.99, 2.99, 0, 15);
    if (_debug)
    {
        Serial.println("----------------------");
        Serial.print("UV Index: ");
        Serial.print(UVindex);
        Serial.println(" mW/cm^2");
        // Serial.println("----------------------");
    }
    return UVindex;
}

float InfiSense::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int InfiSense::readRainSensor()
{
    int rainRaw = analogRead(RAIN_MEASURE);
    int rainIntensity = map(rainRaw, 1024, 0, 0, 100);
    if (_debug)
    {
        Serial.println("----------------------");
        Serial.print("Rain Intensity: ");
        Serial.print(rainIntensity);
        Serial.println(" %");
    }
}

float InfiSense::readBMEAirHumidity()
{
    float Air_Hum;
    Adafruit_BME280 bme;
    boolean status = bme.begin();
    if (!status)
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        return 9999;
    }
    Air_Hum = bme.readHumidity();
    if (_debug)
    {
        Serial.println("----------------------");
        Serial.print("BME Air Humidity = ");
        Serial.print(Air_Hum);
        Serial.println(" %");
    }
    return Air_Hum;
}

float InfiSense::readBMEAirTemp()
{
    float Air_Temp;
    Adafruit_BME280 bme;
    boolean status = bme.begin();
    if (!status)
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        return 9999;
    }
    Air_Temp = bme.readTemperature();
    if (_debug)
    {
        Serial.println("----------------------");
        Serial.print("BME Air Humidity = ");
        Serial.print(Air_Temp);
        Serial.println(" *C");
    }
    return Air_Temp;
}

float InfiSense::readBMEAirPressure()
{
    float Air_Press;
    Adafruit_BME280 bme;
    boolean status = bme.begin();
    if (!status)
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        return 9999;
    }
    Air_Press = bme.readPressure();
    if (_debug)
    {
        Serial.println("----------------------");
        Serial.print("BME Air Pressure = ");
        Serial.print(Air_Press);
        Serial.println(" hPa");
    }
    return Air_Press;
}

float InfiSense::readBMEAltitude()
{
    float Altitude;
    Adafruit_BME280 bme;
    boolean status = bme.begin();
    if (!status)
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        return 9999;
    }
    Altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    if (_debug)
    {
        Serial.println("----------------------");
        Serial.print("BME Altitude = ");
        Serial.print(Altitude);
        Serial.println(" %");
    }
    return Altitude;
}

float InfiSense::readBH1750()
{
    return (InfiSense::readLUXSensor());
}

float InfiSense::readLUXSensor()
{
    float LUX;
    BH1750 lightMeter;
    Wire.begin();
    lightMeter.begin();
    LUX = lightMeter.readLightLevel();
    delay(500);
    if (_debug)
    {
        Serial.println("----------------------");
        Serial.print("Light: ");
        Serial.print(LUX);
        Serial.println(" lx");
        // Serial.println("----------------------");
    }
    return LUX;
}

void InfiSense::triggerRelayOn()
{
    InfiSense::triggerPumpOn();
}

void InfiSense::triggerRelayOff()
{
    InfiSense::triggerPumpOff();
}

void InfiSense::triggerPumpOn()
{
    digitalWrite(RELAY_PIN, HIGH);
    if (_debug)
        Serial.println("Pump is turned ON!!");
}

void InfiSense::triggerPumpOff()
{
    digitalWrite(RELAY_PIN, LOW);
    if (_debug)
        Serial.println("Pump is turned OFF!!");
}

float InfiSense::readBatteryVoltage()
{
    ads.begin();
    float Batt_raw = ads.readADC_SingleEnded(3);
    float BatteryVoltage = (Batt_raw / 32768) * 6.144;
    if (_debug)
    {
        Serial.println("----------------------");
        Serial.print("Battery Voltage: ");
        Serial.print(BatteryVoltage);
        Serial.println(" Volts");
        // Serial.println("----------------------");
    }
    return BatteryVoltage;
}

void InfiSense::turnLedOn()
{
    InfiSense::triggerLedOn();
}

void InfiSense::triggerLedOn()
{
    digitalWrite(LED_PIN, HIGH);
    if (_debug)
        Serial.println("LED is turned ON!!");
}

void InfiSense::turnLedOff()
{
    InfiSense::triggerLedOff();
}

void InfiSense::triggerLedOff()
{
    digitalWrite(LED_PIN, LOW);
    if (_debug)
        Serial.println("LED is turned OFF!!");
}

