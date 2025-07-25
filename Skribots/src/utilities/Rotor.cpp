#include <Arduino.h>
#include "Rotor.h"

Rotor::Rotor(int SpeedPin,int DirectionPin){
	#ifndef _VARIANT_BBC_MICROBIT_  
	_speed_pin 	= SpeedPin;
	_dir_pin 	= DirectionPin;
	SetNewPWMChannel(_speed_pin);
	SetNewPWMChannel(_dir_pin);
	pinMode(_dir_pin,OUTPUT);
	#else
	_dir_pin_1 = SpeedPin;
  	_dir_pin_2 = DirectionPin;
  	pinMode(_dir_pin_1, OUTPUT);
  	pinMode(_dir_pin_2, OUTPUT);
	#endif
	speed_scale = 100;
	SetSpeed(100);
	SetDirection(1);
	_c_dir = -1;
	_c_speed = -1;
	invert = false;
	stopped = true;
}

void Rotor::SetSpeed(int speed){
	if (speed < 0) speed = 0;
		_speed = speed_scale*speed/100; 
}

void Rotor::SetSpeedRaw(int speed) {
	_speed = 15 + speed * 240 / 100;
}
void Rotor::Move(){
stopped = false;
 
if(_c_speed != _speed || _c_dir != _dir){	
		PWM_Write(_dir_pin,0);
		PWM_Write(_speed_pin,0);	
	if(_dir){
		PWM_Write(_dir_pin,255);
		PWM_Write(_speed_pin,255-_speed);
	}else{
		PWM_Write(_speed_pin,255);
		PWM_Write(_dir_pin,255-_speed);
	}
	_c_dir = _dir;
	_c_speed = _speed;
}

}

void Rotor::invert_rotor(bool inv){
	invert = inv;
}

void Rotor::Stop(){
	if(!stopped){
	#ifndef _VARIANT_BBC_MICROBIT_ 
		PWM_Write(_dir_pin,0);
		PWM_Write(_speed_pin,0);	
	#else
		digitalWrite(_dir_pin_1,LOW);
		digitalWrite(_dir_pin_2,LOW);	
	#endif
		_c_speed = -1;
		_c_dir = -1;
		stopped = true;
	}
}

void Rotor::SetDirection(int tm_dir){
	_dir = tm_dir;
	if(invert)_dir=abs(_dir-1);
}

int Rotor::GetSpeed(){
	return(_speed);
}

void Rotor::scale_speed(byte _scale){
	 speed_scale = _scale;
}

int Rotor::GetDirection(){
	return(_dir);
}