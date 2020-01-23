#include "InfiSense.h"

InfiSense agri;

boolean DEBUG = false;

void setup()
{
  Serial.begin(9600);

  agri.begin(DEBUG);
}

void loop()
{
  // Serial.println(agri.begin(DEBUG));
  Serial.println(agri.readSoilSensor());
  delay(10);
}