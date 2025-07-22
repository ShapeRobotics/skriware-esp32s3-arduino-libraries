#include <Arduino.h>
#include "PWM_Handler.h"

  #define SLOW_PWM_TIMEOUT 30
  byte used_PWM_channels = 0;
  PWM_Channel PWM_Channels[MAX_PWM_CHANNELS];
  bool PWM_overload = false;

  void PWM_Write(byte _PWMPin,int value){
      for(byte ii = 0; ii < MAX_PWM_CHANNELS;ii++){
        if(PWM_Channels[ii].pwmPin == _PWMPin){
        #ifdef ESP_H
          ledcWrite(PWM_Channels[ii].pwmChannel, value);
          PWM_Channels[ii].value = value;
        #else 
          analogWrite(PWM_Channels[ii].pwmPin,value);
        #endif
        break;
        }
      }
  }

  void software_Slow_PWM_Write(byte _PWMPin,int value){
      for(byte ii = 0; ii < MAX_PWM_CHANNELS;ii++){
        if(PWM_Channels[ii].pwmPin == _PWMPin){
        #ifdef ESP_H
          slow_PWM_write(PWM_Channels[ii].value,value,SLOW_PWM_TIMEOUT,PWM_Channels[ii]);
          PWM_Channels[ii].value = value;
        #else 
          analogWrite(PWM_Channels[ii].pwmPin,value);
        #endif
        break;
        }
      }
  }

  void slow_PWM_write(byte startingValue, byte endValue, int _delay,PWM_Channel pwm) {
      if(startingValue == endValue) return;
      if (startingValue > endValue) {
          byte steps    =  (startingValue - endValue) / 5;
          byte div = (startingValue - endValue) / steps;
          for (int ii = 0; ii < steps; ii++) {
              ledcWrite(pwm.pwmChannel, startingValue - div * ii);
              delay(_delay);
          }
      }
      else {
          byte steps = (endValue - startingValue) / 5;
          byte div = (endValue - startingValue) / steps;
          for (int ii = 0; ii < steps; ii++) {
              ledcWrite(pwm.pwmChannel, startingValue + div * ii);
              delay(_delay);
          }
      }
  }


  int SetNewPWMChannel(byte _PWMPin, byte resolution, int freq) {
    PWM_Channel newChannelToUse;
    if(used_PWM_channels+1 > MAX_PWM_CHANNELS){
      PWM_overload = true;
      return -1;
    }else{
          newChannelToUse.pwmPin = _PWMPin;
          newChannelToUse.pwmChannel = used_PWM_channels;
          PWM_Channels[used_PWM_channels] = newChannelToUse;
          used_PWM_channels++;
        #ifdef ESP_H
          ledcAttachChannel(newChannelToUse.pwmPin,freq, resolution, newChannelToUse.pwmChannel); 
        #endif
        #if defined(_VARIANT_BBC_MICROBIT_) || defined(ARDUINO_AVR_MEGA2560)
          pinMode(_PWMPin, OUTPUT);
        #endif
      return used_PWM_channels - 1;
    }

  }

  void analogWrite(byte pin, byte value) {
      bool pinExists = false;
      for (int uu = 0; uu < MAX_PWM_CHANNELS; uu++) {
          if (PWM_Channels[uu].pwmPin == pin) {
              pinExists = true;
              break;
          }
      }
          if (!pinExists)SetNewPWMChannel(pin);
          PWM_Write(pin, value);

  }

  bool PWM_overloaded(){
    return(PWM_overload);
  }

  void clearPWM(){
    for(byte tt = 0; tt < used_PWM_channels;tt++){
      ledcDetach(PWM_Channels[tt].pwmPin);
    }
    used_PWM_channels = 0;
  }

bool PWM_defined(byte pin)
{
  for (byte i = 0; i < used_PWM_channels; i++)
  {
    if (PWM_Channels[i].pwmPin == pin)
      return true;
  }
  return false;
}