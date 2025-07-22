#include <Arduino.h>
#include "Gyroscope.h"

	Gyroscope::Gyroscope(byte SDA_pin,byte CLK_pin,byte id){
		_i2c = new I2CHandler(SDA_pin,CLK_pin);
		X_minVal = DAFAULT_X_min;
		X_maxVal = DAFAULT_X_max;
		Y_minVal = DAFAULT_Y_min;
		Y_maxVal = DAFAULT_Y_max;
		Z_minVal = DAFAULT_Z_min;
		Z_maxVal = DAFAULT_Z_max;

	}
	Gyroscope::~Gyroscope(){
		delete _i2c;
	}
	bool Gyroscope::init(){
		byte init_msg[] = {0x20,0x6B,0};
		if(!_i2c->I2Cscan(MPU_ADDR))return(false);
		if(!_i2c->I2CTransfer(init_msg,MPU_ADDR))return(false);
		return(true);
	}
	int Gyroscope::read_X_pos(){
			read_XYZ_pos();
			return(X);
	}
	int Gyroscope::read_Y_pos(){
			read_XYZ_pos();
			return(Y);
	}
	int Gyroscope::read_Z_pos(){
			read_XYZ_pos();
			return(Z);
	}

	void Gyroscope::read_XYZ_pos(){
		read_raw_data();
		int xAng = map(raw_x,X_minVal,X_maxVal,-90,90);
  		int yAng = map(raw_y,Y_minVal,Y_maxVal,-90,90);
  		int zAng = map(raw_z,Z_minVal,Z_maxVal,-90,90);
  		X = RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  		Y = RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  		Z = RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
	}
	void Gyroscope::read_raw_data(bool info){
		byte read_msg[] = {0x1E,0x3B};
		_i2c->I2CTransfer(read_msg,MPU_ADDR);
		int AcX=_i2c->output_buffer[0]<<8|_i2c->output_buffer[1];
  		int AcY=_i2c->output_buffer[2]<<8|_i2c->output_buffer[3];
        int AcZ=_i2c->output_buffer[4]<<8|_i2c->output_buffer[5];
  		raw_x = AcX;
  		raw_y = AcY;
  		raw_z = AcZ;
  		if(raw_x > 20000)raw_x = raw_x-65535;
  		if(raw_y > 20000)raw_y = raw_y-65535;
  		if(raw_z > 20000)raw_z = raw_z-65535;  //scaling to make readout continous
  		if(info){

  			  Serial.print(raw_x);
  			  Serial.print("\t");
			  Serial.print(raw_y);
			  Serial.print("\t");
			  Serial.print(raw_z);
			  Serial.println();
			
  		}
	}
	void Gyroscope::set_calibrate_params(float _X_minVal,float _X_maxval,float _Y_minVal,float _Y_maxval,float _Z_minVal,float _Z_maxval){
		X_minVal = _X_minVal;
		X_maxVal = _X_maxval;
		Y_minVal = _Y_minVal;
		Y_maxVal = _Y_maxval;
		Z_minVal = _Z_minVal;
		Z_maxVal = _Z_maxval;
	}

	void Gyroscope::calibrate(){
		int _X_minVal = 0xFF;
		int _X_maxVal = -0xFF;
		int _Y_minVal = 0xFF;
		int _Y_maxVal = -0xFF;
		int _Z_minVal = 0xFF;
		int _Z_maxVal = -0xFF;
		delay(5000);
		while(true){
			Serial.println("Instruction:\n 1. Type \"OK\" and close this window.\n 2. Navigate to \"Serial Plotter\".\n 3. Type \"Start\" in the terminal of the ploter.\n 4. Move the device around the XYZ axis slowly to cover the full angle on every Axis(confirm on the ploter). \n 5. Confirm with \"OK\" in the terminal window when you've reached all maximums a minimums.\n 6. exit Serial plotter and run Serial monitor again to obtain calibration parameters.");
			delay(5000);
			if(Serial.available()){
				Serial.readString();
				break;
			}
		}
		while(true){
			if(Serial.available()){
				Serial.readString();
				break;
			}
		}
		while(true){
			if(Serial.available()){
				Serial.readString();
				break;
			}
			read_raw_data();
			delay(100);
			if(abs(raw_x) <25000 && abs(raw_y) < 25000 && abs(raw_z)<25000){
			_X_maxVal = (raw_x > _X_maxVal) ? raw_x : _X_maxVal;
			_X_minVal = (raw_x > _X_minVal) ? _X_minVal : raw_x;
			_Y_maxVal = (raw_y > _Y_maxVal) ? raw_y : _Y_maxVal;
			_Y_minVal = (raw_y > _Y_minVal) ? _Y_minVal : raw_y;
			_Z_maxVal = (raw_z > _Z_maxVal) ? raw_z : _Z_maxVal;
			_Z_minVal = (raw_z > _Z_minVal) ? _Z_minVal : raw_z;
		 }
			  Serial.print(raw_x);
  			  Serial.print("\t");
			  Serial.print(raw_y);
			  Serial.print("\t");
			  Serial.print(raw_z);
			  Serial.println();
		}
		while(true){
		Serial.println("Calibration compleate. Put the following line in the \"setup\" function of your gyroscope code:");
		Serial.print("robot.Set_gyroscope_calibration_parameters(");
		Serial.print(_X_minVal);
		Serial.print(",");
		Serial.print(_X_maxVal);
		Serial.print(",");
		Serial.print(_Y_minVal);
		Serial.print(",");
		Serial.print(_Y_maxVal);
		Serial.print(",");
		Serial.print(_Z_minVal);
		Serial.print(",");
		Serial.print(_Z_maxVal);
		Serial.println(");");
		delay(5000);
		}

		X_minVal = _X_minVal;
		X_maxVal = _X_maxVal;
		Y_minVal = _Y_minVal;
		Y_maxVal = _Y_maxVal;
		Z_minVal = _Z_minVal;
		Z_maxVal = _Z_maxVal;

	}