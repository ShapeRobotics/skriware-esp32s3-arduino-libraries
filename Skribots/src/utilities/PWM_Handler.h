#ifndef PWM_HANDLE_H
#define PWM_HANDLE_H
#include "Arduino.h"
#ifdef ESP_H
#include "esp32-hal-ledc.h"
#define MAX_PWM_CHANNELS 16
#define PWM_FREQ 350
#define PWM_RESOLUTION 8 //in bits
#endif
#ifdef _VARIANT_BBC_MICROBIT_
#define MAX_PWM_CHANNELS 3
#endif
#ifdef ARDUINO_AVR_MEGA2560
#define MAX_PWM_CHANNELS 5
#endif


  struct PWM_Channel{
    byte pwmPin;
    byte pwmChannel;
    byte value;
  };
  void PWM_Write(byte PWMPin,int value);
  void slow_PWM_write(byte startingValue, byte endValue, int _delay,PWM_Channel pwm);
  void software_Slow_PWM_Write(byte _PWMPin,int value);
  int SetNewPWMChannel(byte PWMPin, byte resolution = PWM_RESOLUTION, int freq = PWM_FREQ);
  bool PWM_overloaded();
  void clearPWM();
  void analogWrite(byte pin,byte value);
  bool PWM_defined(byte pin);
#endif