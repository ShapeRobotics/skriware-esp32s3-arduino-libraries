#ifndef Claw_H
#define Claw_H
#include "Arduino.h"
#include "PWM_Handler.h"
#ifndef ESP_H
#include <Servo.h>
#endif

#define BV_0_up	50
#define BV_0_down 10
#define BV_0_close 120
#define BV_0_open 170

#define BV_1_up	120
#define BV_1_down 70
#define BV_1_close 70
#define BV_1_open 120

#define BV_2_up 85
#define BV_2_down 45
#define BV_2_close 45
#define BV_2_open 90

class Claw
{
public:
	Claw(int Claw_Pin,int Arm_Pin,byte id = 0);
	void SetAngle(int claw_angle,int arm_angle);
	void SetPositions(byte _up_v,byte _down_v,byte _open_v,byte _close_v);
	void Close();
	void Open();
	void Pick_Up();
	void Put_Down();
	void updateClaw();
	byte GetID();
private:
	#ifndef ESP_H
	Servo 	*claw_servo;
	Servo 	*arm_servo;
	#endif
	bool tmp;
	byte claw_pin;
	byte arm_pin;
	byte id;
	byte up_v,down_v,open_v,close_v;
};

#endif