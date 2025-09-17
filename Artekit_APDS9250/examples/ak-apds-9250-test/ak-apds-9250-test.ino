#include <Wire.h>
#include "Artekit_APDS9250.h"

Artekit_APDS9250 myApds9250;

void setup() {
  Serial.begin(9600);

  if (myApds9250.begin(36,35,0))
  {
    Serial.println("begin() OK");
  }

  Serial.print("setMode():");
  Serial.println((int) modeColorSensor);
  myApds9250.setMode(modeColorSensor);
  Serial.print("Mode: ");
  Serial.println((int) myApds9250.getMode());

  Serial.print("setMode():");
  Serial.println((int) modeAmbientLightSensor);
  myApds9250.setMode(modeAmbientLightSensor);
  Serial.print("Mode: ");
  Serial.println((int) myApds9250.getMode());

  for (int i = 0; i <= res13bit; i++)
  { 
    Serial.print("setResolution():");
    Serial.println(i);
    myApds9250.setResolution(i);
    Serial.print("Resolution: ");
    Serial.println((int) myApds9250.getResolution());
  }

  for (int i = 0; i <= rate2000ms; i++)
  { 
    Serial.print("setRate():");
    Serial.println(i);
    myApds9250.setMeasurementRate(i);
    Serial.print("Rate: ");
    Serial.println((int) myApds9250.getMeasurementRate());
  }
 
  for (int i = 0; i <= gain18; i++)
  { 
    Serial.print("setGain():");
    Serial.println(i);
    myApds9250.setGain(i);
    Serial.print("Gain: ");
    Serial.println((int) myApds9250.getGain());
  }

  myApds9250.setMode(modeColorSensor);
  myApds9250.setResolution(res18bit);
  myApds9250.setGain(gain1);
  myApds9250.setMeasurementRate(rate100ms);
}

void loop()
{
  uint32_t r, g, b, ir;
  myApds9250.getAll(&r, &g, &b, &ir);
  char str[64];
  sprintf(str, "r:%lu g:%lu b:%lu ir:%lu", r, g, b, ir);  
  Serial.println(str);
  
  delay(1000);
}
