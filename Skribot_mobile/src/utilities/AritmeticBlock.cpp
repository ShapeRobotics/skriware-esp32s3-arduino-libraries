#include "AritmeticBlock.h"

AritmeticBlock::~AritmeticBlock(){}

AritmeticBlock::AritmeticBlock(byte _id, byte _operation,byte _left_id,byte _rigth_id){
	blockID = _id;
	leftID 	= _left_id;
	rightID = _rigth_id;
	operation = _operation;
	input_left 	= NULL;
	input_right = NULL;
  inputblockID = 0;
  outputblockID = 0;
  nextblockID = 0;
  N_output = 1;
}

byte AritmeticBlock::get_output_N(){
     if (input_left->get_output_N() > 1 || input_right->get_output_N() > 1) {
            N_output = input_left->get_output_N()+input_right->get_output_N();
        }else{
          N_output = 1;
        }
     return(N_output);
}
  bool AritmeticBlock::set_connections(Block *blockList[],int blockList_N){
  		for(int ii = 0; ii < blockList_N; ii++){
			     if(blockList[ii]->getID() == leftID){
			            input_left = blockList[ii];
			      }
            if(blockList[ii]->getID() == rightID){
			            input_right = blockList[ii];
			      }
  		}       
      if(input_left != NULL && input_right != NULL){
        return(true);
      }else{
        return(false);
      }
  }
  int32_t AritmeticBlock::get_output(){
  	int32_t val_right,val_left;

  	if(input_left   != NULL)val_left    = input_left->get_output();
	if(input_right  != NULL)val_right   = input_right->get_output();

	switch (operation) {
    case 1:
      output = val_left + val_right;
      break;
    case 2:
      output = val_left - val_right;
      break;
    case 3:
      output = val_left * val_right;
      break;
    case 4:
      output = val_left / val_right;
      break;
    case 5:
      output = val_left % val_right;
      break;
    case 6:
      output = pow(val_left,val_right);
      break;
    default:
      output = 0;
  	}
    #ifdef DEBUG_MODE
  	Serial.print("Aritmetic output:");
  	Serial.println(output);
    #endif

  	return(output);
  

  }


  int32_t* AritmeticBlock::get_table_output() {
      int32_t* tmp_table = new int32_t[input_left->get_output_N() + input_right->get_output_N()];
      if (input_left->get_output_N() == 1) {
          tmp_table[0] = input_left->get_output();
      }
      else {
          for (int yy = 0; yy < input_left->get_output_N(); yy++)tmp_table[yy] = input_left->get_table_output()[yy];
      }

      if (input_right->get_output_N() == 1) {
          tmp_table[input_left->get_output_N()] = input_right->get_output();
      }
      else {
          for (int yy = 0; yy < input_left->get_output_N(); yy++)tmp_table[input_left->get_output_N() + yy] = input_right->get_table_output()[yy];
      }
      set_table_output(tmp_table, input_left->get_output_N() + input_right->get_output_N());
      return(t_output);
  }

  byte* AritmeticBlock::get_table_output_8() {
      get_table_output();
      if (N_output > 0) {
          uint8_t* tmp = new uint8_t[N_output];
          for (byte rr = 0; rr < N_output;rr++) {
              tmp[rr] = (uint8_t)t_output[rr];

          }
          return(tmp);
      }
      else {
          return(NULL);
      }
  }
