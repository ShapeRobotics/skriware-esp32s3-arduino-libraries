#ifndef AritmeticBlock_H
#define AritmeticBlock_H
#include "Block.h"
class AritmeticBlock : public Block {

public:
  AritmeticBlock(byte id, byte operation  ,byte left_id,byte rigth_id);
  bool set_connections(Block *blockList[],int blockList_N);
  int32_t get_output();
  int32_t* get_table_output();
  byte* get_table_output_8();
  byte get_output_N();
  ~AritmeticBlock(); 

 protected:
 	byte leftID,rightID,operation;
 	Block *input_left;
 	Block *input_right;
 	

};

#endif