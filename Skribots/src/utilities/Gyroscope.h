#ifndef GYROSCOPE_H
#define GYROSCOPE_H
#include "Arduino.h"
#include "I2CHandler.h"

#define DAFAULT_X_min -17071
#define DAFAULT_X_max 17684
#define DAFAULT_Y_min -16423
#define DAFAULT_Y_max 16620
#define DAFAULT_Z_min -20035
#define DAFAULT_Z_max 17724

#define MPU_ADDR 0x68

class Gyroscope{
public:
	Gyroscope(byte SDA_pin,byte CLK_pin,byte id);
	~Gyroscope();
	bool init();
	int read_X_pos();
	int read_Y_pos();
	int read_Z_pos();
	void read_XYZ_pos();
	void read_raw_data(bool info = false);
	void set_calibrate_params(float _X_minVal,float _X_maxval,float _Y_minVal,float _Y_maxval,float _Z_minVal,float _Z_maxval);
	void calibrate();
private:
	I2CHandler *_i2c;
	float X_minVal,X_maxVal,Y_minVal,Y_maxVal,Z_minVal,Z_maxVal;
	float raw_x,raw_y,raw_z;
	int X,Y,Z;

};
#endif