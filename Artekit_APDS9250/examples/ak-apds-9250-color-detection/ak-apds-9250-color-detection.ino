#include <Wire.h>
#include <Artekit_APDS9250.h>

#define PIN_RED     5
#define PIN_GREEN   4
#define PIN_BLUE    3
#define PIN_IR      2

Artekit_APDS9250 myApds9250;

void setup() {

  pinMode(PIN_RED,OUTPUT);
  pinMode(PIN_GREEN,OUTPUT);
  pinMode(PIN_BLUE,OUTPUT);
  pinMode(PIN_IR,OUTPUT);

  digitalWrite(PIN_RED,HIGH);
  digitalWrite(PIN_GREEN,HIGH);
  digitalWrite(PIN_BLUE,HIGH);
  digitalWrite(PIN_IR,HIGH);
  
  Serial.begin(9600);
  Serial.println("Serial OK");
  if (myApds9250.begin())
  {
    Serial.println("begin() OK");
  } else {
    Serial.println("begin() FAIL");
  }

  myApds9250.setMode(modeColorSensor);
  myApds9250.setResolution(res18bit);
  myApds9250.setGain(gain9);
  myApds9250.setMeasurementRate(rate100ms);
}

void loop()
{
  uint32_t r, g, b, ir;
  myApds9250.getAll(&r, &g, &b, &ir);
  char str[64];
  sprintf(str, "%lu %lu %lu %lu", b, r, g, ir);  
  Serial.println(str);

  if (r > 3000) {
    digitalWrite(PIN_RED,LOW);
  } else {
    digitalWrite(PIN_RED,HIGH);
  }

  if (g > 2500) {
    digitalWrite(PIN_GREEN,LOW);
  } else {
    digitalWrite(PIN_GREEN,HIGH);
  }

  if ((b > 1000) && (g < 1000) && (r < 1000)) {
    digitalWrite(PIN_BLUE,LOW);
  } else {
    if ((r > 5000) && (g > 12000) && (b > 10000)) {
      digitalWrite(PIN_BLUE,LOW);
    } else {
      digitalWrite(PIN_BLUE,HIGH);
    }
  }

  if (ir > 1000) {
    digitalWrite(PIN_IR,LOW);
  } else {
    digitalWrite(PIN_IR,HIGH);
  }

  delay(250);
}
