#ifndef Rotor_H
#define Rotor_H
#include "Arduino.h"
#include "PWM_Handler.h"
  class Rotor
 {
  public:
  	Rotor(int SpeedPin,int DirectionPin);
    void SetSpeed(int speed);
    void SetSpeedRaw(int speed);
    void SetDirection(int dir);
    void Move();
    void Stop();
    int GetSpeed();
    int GetDirection();
    void invert_rotor(bool inv = true);
    void scale_speed(byte _scale);
  #ifndef _VARIANT_BBC_MICROBIT_
  byte _speed_pin;
  byte _dir_pin;
  #else
  byte _dir_pin_1;
  byte _dir_pin_2;
  #endif
  bool stopped = true;
  bool invert;
  byte speed_scale;
  int _speed,_c_speed;
  int _dir,_c_dir;
 };

 #endif