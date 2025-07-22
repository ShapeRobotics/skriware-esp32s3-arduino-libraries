#ifndef SBlock_H
#define SBlock_H
#define DEBUG_MODE
#define FIRMWARE_VERSION "3.6"
#define HARDWARE_VERSION "3."
#include "utilities/Block.h"
#include "utilities/Conditional.h"
#include "utilities/Loop.h"
#include "utilities/BlockHandler.h"
#include "utilities/LogicBlock.h"
#include "utilities/ConstBlock.h"
#include "utilities/LogicCompare.h"
#include "utilities/AritmeticBlock.h"
#include "utilities/InterruptHandler.h"


void skribot_setup();
void skribot_loop();
byte Board_Version();
void raport_user_code_mode(byte BV);
void IRAM_ATTR reset_to_user_code();

class Skribot_mobile {
	public:
	Skribot_mobile();
};


#endif