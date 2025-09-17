#include "Artekit_APDS9250.h"
#include <Wire.h>

void Artekit_APDS9250::readRegisters(uint8_t reg, uint8_t* values, uint8_t count)
{
	uint8_t idx = 0;
	long startTime = millis();
	if(usedBus == 0){
		Wire.beginTransmission(APDS9250_ADDRESS);
		Wire.write(reg);
		Wire.endTransmission(false);
		Wire.requestFrom((int) APDS9250_ADDRESS, (int) count, (int) true);
		
		while (count && (millis() - startTime < 30)) // Wait for data or timeout
		{
			if (Wire.available())
			{
				values[idx++] = Wire.read();
				count--;
			}
		}
	}else{
		Wire1.beginTransmission(APDS9250_ADDRESS);
		Wire1.write(reg);
		Wire1.endTransmission(false);
		Wire1.requestFrom((int) APDS9250_ADDRESS, (int) count, (int) true);
	
		while (count && (millis() - startTime < 30)) // Wait for data or timeout
		{
			if (Wire1.available())
			{
				values[idx++] = Wire1.read();
				count--;
			}
		}
		}
}

uint8_t Artekit_APDS9250::readRegister(uint8_t reg)
{
	if(usedBus == 0){
		Wire.beginTransmission(APDS9250_ADDRESS);
		Wire.write(reg);
		Wire.endTransmission(false);
		Wire.requestFrom((int) APDS9250_ADDRESS, (int) 1, (int) true);
		while(!Wire.available());
		return Wire.read();
	}else{
		Wire1.beginTransmission(APDS9250_ADDRESS);
		Wire1.write(reg);
		Wire1.endTransmission(false);
		Wire1.requestFrom((int) APDS9250_ADDRESS, (int) 1, (int) true);
		while(!Wire1.available());
		return Wire1.read();
	}
}

void Artekit_APDS9250::writeRegister(uint8_t reg, uint8_t value)
{
	if(usedBus == 0){
	Wire.beginTransmission(APDS9250_ADDRESS);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission();
	}else{
	Wire1.beginTransmission(APDS9250_ADDRESS);
	Wire1.write(reg);
	Wire1.write(value);
	Wire1.endTransmission();	
	}	
}

bool Artekit_APDS9250::begin(uint8_t scl, uint8_t sda,uint8_t _usedBus)
{
	usedBus = _usedBus;
	if(usedBus == 0){
	Wire.begin(sda,scl);
	}else{
	Wire1.begin(sda,scl);	
	}
	
	// Read PART_ID
	uint8_t part = readRegister(APDS9250_REG_PART_ID);
	if (part >> 4 != 0x0B)
		return false;
	
	if (!setMode(modeAmbientLightSensor))
		return false;

	resolution_mask = getResolutionMask(current_res);
	return enable();
}

bool Artekit_APDS9250::enable()
{
	uint8_t reg = readRegister(APDS9250_REG_MAIN_CTRL);
	reg |= 0x02;
	writeRegister(APDS9250_REG_MAIN_CTRL, reg);
	return true;
}

bool Artekit_APDS9250::disable()
{
	uint8_t reg = readRegister(APDS9250_REG_MAIN_CTRL);
	reg &= ~0x02;
	writeRegister(APDS9250_REG_MAIN_CTRL, reg);
	return true;
}

bool Artekit_APDS9250::reset()
{
	uint8_t reg = readRegister(APDS9250_REG_MAIN_CTRL);
	reg |= 0x10;
	writeRegister(APDS9250_REG_MAIN_CTRL, reg);
	return true;
}

bool Artekit_APDS9250::setMode(APDS9250Mode mode)
{
	uint8_t reg = readRegister(APDS9250_REG_MAIN_CTRL);
	reg &= ~0x04;
	reg |= (mode == modeAmbientLightSensor) ? 0 : 0x04;
	writeRegister(APDS9250_REG_MAIN_CTRL, reg);
	return true;
}

bool Artekit_APDS9250::setResolution(APDS9250Resolution resolution)
{
	uint8_t reg = readRegister(APDS9250_REG_LS_MEAS_RATE);
	reg &= ~0x70;
	reg |= ((uint8_t) resolution) << 4;
	writeRegister(APDS9250_REG_LS_MEAS_RATE, reg);

	current_res = resolution;
	resolution_mask = getResolutionMask(current_res);
	return true;
}

bool Artekit_APDS9250::setMeasurementRate(APDS9250Rate rate)
{
	uint8_t reg = readRegister(APDS9250_REG_LS_MEAS_RATE);
	reg &= ~0x07;
	reg |= ((uint8_t) rate);
	writeRegister(APDS9250_REG_LS_MEAS_RATE, reg);
	return true;
}

bool Artekit_APDS9250::setGain(APDS9250Gain gain)
{
	uint8_t reg = (uint8_t) gain;
	writeRegister(APDS9250_REG_LS_GAIN, reg);
	return true;
}

APDS9250Mode Artekit_APDS9250::getMode()
{
	uint8_t reg = readRegister(APDS9250_REG_MAIN_CTRL);
	reg = (reg >> 2) & 0x01;
	return (APDS9250Mode) reg;
}

APDS9250Resolution Artekit_APDS9250::getResolution()
{
	uint8_t reg = readRegister(APDS9250_REG_LS_MEAS_RATE);
	reg = (reg >> 4) & 0x07;
	return (APDS9250Resolution) reg;
}

APDS9250Rate Artekit_APDS9250::getMeasurementRate()
{
	uint8_t reg = readRegister(APDS9250_REG_LS_MEAS_RATE);
	reg = reg & 0x07;
	return (APDS9250Rate) reg;
}

APDS9250Gain Artekit_APDS9250::getGain()
{
	uint8_t reg = readRegister(APDS9250_REG_LS_GAIN);
	reg = reg & 0x07;
	return (APDS9250Gain) reg;	
}
	
uint8_t Artekit_APDS9250::getStatusRegister()
{
	uint8_t reg = readRegister(APDS9250_REG_MAIN_STATUS);
	return reg;
}

uint32_t Artekit_APDS9250::regTo24bits(uint8_t* values)
{
	uint32_t val;
	
	val = values[0];
	val |= values[1] << 8;
	val |= values[2] << 16;
	
	return val & resolution_mask;
}
	
uint32_t Artekit_APDS9250::getRed()
{
	uint32_t val;
	uint8_t values[3];
	
	readRegisters(APDS9250_REG_LS_DATA_RED0, values, 3);

	return regTo24bits(values);
}

uint32_t Artekit_APDS9250::getGreen()
{
	uint32_t val;
	uint8_t values[3];
	
	readRegisters(APDS9250_REG_LS_DATA_GREEN0, values, 3);

	return regTo24bits(values);
}

uint32_t Artekit_APDS9250::getBlue()
{
	uint32_t val;
	uint8_t values[3];
	
	readRegisters(APDS9250_REG_LS_DATA_BLUE0, values, 3);

	return regTo24bits(values);		
}
	
uint32_t Artekit_APDS9250::getInfrared()
{
	uint32_t val;
	uint8_t values[3];
	
	readRegisters(APDS9250_REG_LS_DATA_IR_0, values, 3);

	return regTo24bits(values);	
}

bool Artekit_APDS9250::getAll(uint32_t* red, uint32_t* green, uint32_t* blue, uint32_t* ir)
{
	uint8_t values[12];
	
	readRegisters(APDS9250_REG_LS_DATA_IR_0, values, 12);

	if (ir)
		*ir = regTo24bits(&values[0]);

	if (green)
		*green = regTo24bits(&values[3]);
	
	if (blue)
		*blue = regTo24bits(&values[6]);
	
	if (red)
		*red = regTo24bits(&values[9]);
	
	return true;
}
		
bool Artekit_APDS9250::setInterruptThreshold(uint32_t upper, uint32_t lower, APDSVariationCount count)
{
	writeRegister(APDS9250_REG_LS_THRES_UP0, upper & 0xFF);
	writeRegister(APDS9250_REG_LS_THRES_UP1, (upper >> 8) & 0xFF);
	writeRegister(APDS9250_REG_LS_THRES_UP2, (upper >> 16) & 0xFF);

	writeRegister(APDS9250_REG_LS_THRES_LOW0, lower & 0xFF);
	writeRegister(APDS9250_REG_LS_THRES_LOW1, (lower >> 8) & 0xFF);
	writeRegister(APDS9250_REG_LS_THRES_LOW2, (lower >> 16) & 0xFF);

	uint8_t reg = (uint8_t) count;
	reg = reg & 0x07;
	writeRegister(APDS9250_REG_LS_THRES_VAR, reg);

	return true;
}

bool Artekit_APDS9250::enableInterrupt(uint32_t pin, void* function, APDSInterruptChannel channel, APDSInterruptMode mode)
{
	if (irq_pin != (uint32_t) -1)
		disableInterrupt();

	pinMode(pin, INPUT);
	
	uint8_t reg = readRegister(APDS9250_REG_INT_CFG);

	// Clear other values
	reg &= ~0x3C;

	reg |= ((uint8_t) channel) << 4;
	reg |= ((uint8_t) mode) << 3;

	writeRegister(APDS9250_REG_INT_CFG, reg);

	// Attach interrupt to pin
	//attachInterrupt(pin, function, FALLING);

	// Enable interrupt
	reg = readRegister(APDS9250_REG_INT_CFG);
	reg |= 0x04;
	writeRegister(APDS9250_REG_INT_CFG, reg);

	reg = readRegister(APDS9250_REG_INT_CFG);
	Serial.println(reg, HEX);

	irq_pin = pin;
	return true;
}

bool Artekit_APDS9250::disableInterrupt()
{
	uint8_t reg = readRegister(APDS9250_REG_INT_CFG);
	reg &= ~0x04;
	writeRegister(APDS9250_REG_INT_CFG, reg);

	detachInterrupt(irq_pin);
	irq_pin = (uint32_t) -1;
	return true;
}
