#include <Arduino.h>
#include<Wire.h>

#define APDS9250_ADDRESS 0x52

#define APDS9250_REG_MAIN_CTRL			0x00
#define APDS9250_REG_LS_MEAS_RATE		0x04
#define APDS9250_REG_LS_GAIN			0x05
#define APDS9250_REG_PART_ID			0x06
#define APDS9250_REG_MAIN_STATUS		0x07
#define APDS9250_REG_LS_DATA_IR_0		0x0A
#define APDS9250_REG_LS_DATA_IR_1		0x0B
#define APDS9250_REG_LS_DATA_IR_2		0x0C
#define APDS9250_REG_LS_DATA_GREEN0		0x0D
#define APDS9250_REG_LS_DATA_GREEN1		0x0E
#define APDS9250_REG_LS_DATA_GREEN2		0x0F
#define APDS9250_REG_LS_DATA_BLUE0		0x10
#define APDS9250_REG_LS_DATA_BLUE1		0x11
#define APDS9250_REG_LS_DATA_BLUE2		0x12
#define APDS9250_REG_LS_DATA_RED0		0x13
#define APDS9250_REG_LS_DATA_RED1		0x14
#define APDS9250_REG_LS_DATA_RED2		0x15
#define APDS9250_REG_INT_CFG			0x19
#define APDS9250_REG_INT_PERSISTENCE	0x1A
#define APDS9250_REG_LS_THRES_UP0		0x21
#define APDS9250_REG_LS_THRES_UP1		0x22
#define APDS9250_REG_LS_THRES_UP2		0x23
#define APDS9250_REG_LS_THRES_LOW0		0x24
#define APDS9250_REG_LS_THRES_LOW1		0x25
#define APDS9250_REG_LS_THRES_LOW2		0x26
#define APDS9250_REG_LS_THRES_VAR		0x27

typedef enum _APDS9250Mode
{
	modeAmbientLightSensor,
	modeColorSensor
} APDS9250Mode;

typedef enum _APDS9250Resolution
{
	res20bit = 0,
	res19bit,
	res18bit,
	res17bit,
	res16bit,
	res13bit	
} APDS9250Resolution;

typedef enum _APDS9250Rate
{
	rate25ms = 0,
	rate50ms,
	rate100ms,
	rate200ms,
	rate500ms,
	rate1000ms,
	rate2000ms
} APDS9250Rate;

typedef enum _APDS9250Gain
{
	gain1 = 0,
	gain3,
	gain6,
	gain9,
	gain18	
} APDS9250Gain;

typedef enum _APDSInterruptChannel
{
	channelInfrared = 0,
	channelGreen = 1,
	channelALS = 1,
	channelRed = 2,
	channelBlue = 3
} APDSInterruptChannel;

typedef enum _APDSInterruptMode
{
	modeThreshold,
	modeVariation
} APDSInterruptMode;

typedef enum _APDSInterruptVariationCount
{
	variation8samples,
	variation16samples,
	variation32samples,
	variation64samples,
	variation1024samples,
} APDSVariationCount;

class Artekit_APDS9250
{
public:
	Artekit_APDS9250() : irq_pin((uint32_t) -1), current_res(res18bit) {}
	u_int8_t usedBus = 0;
	bool begin(uint8_t scl, uint8_t sda,uint8_t busID);
	bool enable();
	bool disable();
	bool reset();
	
	bool setMode(APDS9250Mode mode);
	bool setResolution(APDS9250Resolution resolution);
	bool setMeasurementRate(APDS9250Rate rate);
	bool setGain(APDS9250Gain gain);
	
	APDS9250Mode getMode();
	APDS9250Resolution getResolution();
	APDS9250Rate getMeasurementRate();
	APDS9250Gain getGain();
	
	uint8_t getStatusRegister();
	
	uint32_t getRed();
	uint32_t getGreen();
	uint32_t getBlue();
	uint32_t getInfrared();
	inline uint32_t getAmbientLight() { return getGreen(); }
	
	bool getAll(uint32_t* red, uint32_t* green, uint32_t* blue, uint32_t* ir);
	bool setInterruptThreshold(uint32_t upper, uint32_t lower, APDSVariationCount count);
	bool enableInterrupt(uint32_t pin, void* function, APDSInterruptChannel channel, APDSInterruptMode mode);
	bool disableInterrupt();
	
private:
	uint32_t irq_pin;
	APDS9250Resolution current_res;
	uint32_t resolution_mask;

	uint32_t getResolutionMask(APDS9250Resolution res)
	{
		switch (res)
		{
			case res20bit: return 0xFFFFF;
			case res19bit: return 0x7FFFF;
			case res18bit: return 0x3FFFF;
			case res17bit: return 0x1FFFF;
			case res16bit: return 0xFFFF;
			case res13bit: return 0x1FFF;			
		}

		return 0xFFFFFFFF;
	}
	
	uint8_t readRegister(uint8_t reg);
	void readRegisters(uint8_t reg, uint8_t* values, uint8_t count);
	void writeRegister(uint8_t reg, uint8_t value);
    uint32_t regTo24bits(uint8_t *values);
};
