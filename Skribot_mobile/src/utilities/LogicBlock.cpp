#include "LogicBlock.h"


LogicBlock::LogicBlock(byte id,byte _logics,byte _input_left, byte _input_right){
	input_Left_ID 	= _input_left;
	input_Right_ID 	= _input_right;
	blockID = id;
  operation = _logics;
  input_right = NULL;
  input_left = NULL;
  inputblockID = 0;
  outputblockID = 0;
  nextblockID = 0;
  N_output = 1;
}

LogicBlock::LogicBlock(){
}

LogicBlock::~LogicBlock(){
}

  void LogicBlock::get_value(){
	  if(input_left   != NULL)val_left    = input_left->get_output();
	  if(input_right  != NULL)val_right   = input_right->get_output();

	  switch (operation) {
    case 0:
      bool_output = val_left == val_right;
      break;
    case 3:
      bool_output = val_left > val_right;
      break;
    case 2:
      bool_output = val_left < val_right;
      break;
    case 1:
      bool_output = val_left != val_right;
      break;
    case 4:
        bool_output = val_left >= val_right;
      break;
    case 5:
        bool_output = val_left <= val_right;
      break;
    default:
      bool_output = false;
  	}
  }
  bool LogicBlock::set_logics(Block *blockList[], int blockList_N){
  		for(int ii = 0; ii < blockList_N; ii++){
			     if(blockList[ii]->getID() == input_Left_ID){
			            input_left = blockList[ii];
			      }
            if(blockList[ii]->getID() == input_Right_ID){
			            input_right = blockList[ii];
			      }
  		}
      if(input_right != NULL && input_left != NULL){
        return(true);
      }else{
        return(false);
      }
  }

  bool LogicBlock::get_bool_output(){
    get_value();
    return(bool_output);
  }

  int32_t LogicBlock::get_output(){
    return((int32_t)get_bool_output() == true);
  }

  void LogicBlock::set_output(int32_t x){
    set_bool_output(x != 0);
    output = x;
    bool_output =  x != 0;
    N_output = 1;
  }
