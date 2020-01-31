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

#include "InfiCloud.h"
#include <Wire.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

InfiCloud::InfiCloud(char *token)
{
    _token = token;
    _projectName = DEFAULT_PROJECT_NAME;
    maxValues = 10;
    currentValue = 0;
    val = (Value *)malloc(maxValues * sizeof(Value));
    pinMode(LEDpin, OUTPUT);
    InfiCloud::idAsMac();
}

void InfiCloud::idAsMac()
{
    unsigned char mac[6];
    WiFi.macAddress(mac);
    for (int i = 0; i < 6; i++)
    {
        if ((int)mac[i] < 10)
            sprintf(_espID, "%s0%X", _espID, mac[i]);
        else
            sprintf(_espID, "%s%2X", _espID, mac[i]);
    }
}

void InfiCloud::setDebug(bool debug)
{
    _debug = debug;
}

/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */

void InfiCloud::add(char *variable_id, float value)
{
    return add(variable_id, value, NULL, NULL);
}
void InfiCloud::add(char *variable_id, float value, char *ctext)
{
    return add(variable_id, value, ctext, NULL);
}
void InfiCloud::add(char *variable_id, float value, unsigned long timestamp)
{
    return add(variable_id, value, NULL, timestamp);
}
void InfiCloud::add(char *variable_id, float value, char *ctext, unsigned long timestamp)
{
    (val + currentValue)->id = variable_id;
    (val + currentValue)->value_id = value;
    currentValue++;
    if (currentValue > maxValues)
    {
        Serial.println(F("You are sending more than 10 consecutives variables, you just could send 5 variables. Then other variables will be deleted!"));
        currentValue = maxValues;
    }
}

String InfiCloud::makePayload()
{
    uint16_t i;

    all = "";
    str = "";
    all = "{";
    all += "{\"token\":\"";
    all += String(_token);
    all += "\", \"projname\":\"";
    all += _projectName;
    all += "\", \"Macid\":\"";
    all += String(_espID);
    all += "\"}, ";
    for (i = 0; i < currentValue;)
    {
        str = String(((val + i)->value_id), 4); //variable value
        all += "{\"variable\": \"";
        all += String((val + i)->id); //variable name
        all += "\", \"value\":";
        all += str;
        all += "}";
        i++;
        if (i < currentValue)
        {
            all += ", ";
        }
    }
    all += "}";
    i = all.length();

    if (_debug)
    {
        Serial.println("-------------------------------");
        Serial.print("Payload ::");
        Serial.println(all);
        Serial.print("Payload Length ::");
        Serial.println(i);
        // Serial.println("-----------------");
    }
    currentValue = 0;
    return all;
}

bool InfiCloud::attachLEDToStatus(bool status)
{
    _ledForDebug = status;
    if (_debug)
    {
        if (_ledForDebug)
            Serial.println("LED is attached to WiFi status...");
        else
            Serial.println("LED is detached to WiFi status...");
    }
}

int InfiCloud::connectToWiFi(char *_ssid, char *_pass)
{
    bool _status = false;
    WiFi.mode(WIFI_OFF); //for Preventing reconnection issue
    delay(10);
    WiFi.mode(WIFI_STA); //hides the viewing of device as wifi hotspot
    if (_debug)
        Serial.println("Scanning for WiFI...");
    WiFi.begin(_ssid, _pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        if (_debug)
        {
            Serial.print(".");
        }
        if (_ledForDebug)
        {
            _status = !_status;
            digitalWrite(LEDpin, _status);
        }
    }
    Serial.println("");
    if (WiFi.status() == WL_CONNECTED)
    {
        digitalWrite(LEDpin, HIGH);
    }
    WiFi.setAutoReconnect(true);
    Serial.println(F("WiFi connected"));
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP()); //IP address assigned to your ESP
}

int InfiCloud::sendToCloud(char *_host)
{
    HTTPClient http; //object of class HTTPClient

    http.begin(_host); //Specify request destination

    http.addHeader("Content-Type", "application/json"); //Specify content-type header

    int httpCode = http.POST(all);      //Send the request
    String response = http.getString(); //Get the response payload

    Serial.print("Response Code::  ");
    Serial.println(httpCode); //Print HTTP return code
    Serial.print("Response::  ");
    Serial.println(response); //Print request response payload
    Serial.println("-------------------------------");
    Serial.println();

    http.end(); //Close connection

    if (httpCode == 200)
        dataSentLED();
    else
        dataNotSentLED();
        return httpCode;
}

void InfiCloud::ledOn()
{
    digitalWrite(LEDpin, HIGH);
}
void InfiCloud::ledOff()
{
    digitalWrite(LEDpin, LOW);
}

void InfiCloud::dataSentLED()
{
    ledOff();
    delay(500);
    ledOn();
    delay(500);
    ledOff();
    delay(200);
    ledOn();
    delay(200);
}

void InfiCloud::dataNotSentLED()
{
    ledOff();
    delay(500);
    ledOn();
    delay(500);
    ledOff();
    delay(1000);
    ledOn();
    delay(1000);
}