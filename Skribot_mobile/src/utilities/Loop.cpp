#include "Loop.h"

Loop::Loop() {
  counter = 0;
}

Loop::~Loop() {}

Loop::Loop(byte id, byte _begin, byte _end, byte _limit) {
  blockID = id;
  actionID = 255;
  limit_block_id = _limit;
  begin_block_id = _begin;
  end_block_id = _end;
  input_block = NULL;
  next = NULL;
  output_block = NULL;
  limit_block = NULL;
  end_block = NULL;
  begin_block = NULL;
  counter = 0;
  inputblockID = 0;
  outputblockID = 0;
  nextblockID = 0;
  N_output = 0;

}

Loop::Loop(byte _limit){
  limit = _limit;
  counter = 0;
 
}

void Loop::do_action() {
  counter++;
}
bool Loop::set_connections(Block* blockList[],int blockList_N){
for(byte ii = 0; ii < blockList_N; ii++){
     if(blockList[ii]->getID() == begin_block_id){
            begin_block = blockList[ii];
      }else if(blockList[ii]->getID() == end_block_id){
            end_block = blockList[ii];
      }else if(blockList[ii]->getID() == limit_block_id){
        set_limit_block(blockList[ii]);
        set_limit();
      }
  
  }

  if(begin_block != NULL && (end_block != NULL || end_block_id == 0)  && limit_block != NULL){
    return(true);
  }else{
    return(false);
  }
}

byte Loop::get_begin_block_id(){
  return(begin_block_id);
}
byte Loop::get_limit_block_id(){
  return(limit_block_id);
}

byte Loop::get_end_block_id(){
  return(end_block_id);
}

void Loop::set_begin_block(Block *b){
  begin_block = b;
}

void Loop::set_end_block(Block *b){
  end_block = b;
}

void Loop::set_limit_block(Block *l){
  limit_block =l;
}

void Loop::set_limit(){
if(limit_block->get_output() == -100){
    limit = -1;
    Block::robot->ignore_connection_break = true;
  }else{
    limit = limit_block->get_output();
  }
}
Block* Loop::get_next() {
  if(limit_block->is_const_block()){
            counter++;
            if(limit_block->get_output_N() > 1){
              if(counter <= limit_block->get_output_N()){
                return(begin_block);
              }else{
                counter  = 0;
                return(end_block);
              }
            }else{
                if (counter <= limit_block->get_output() || limit_block->get_output() == -1){
                  return(begin_block);
                }
                else{
                  counter  = 0;
                  return(end_block);
                }
            }
  }else{
            if(limit_block->get_bool_output()){
              return(begin_block);
            }else{
              return(end_block);
            }
  }
}

int32_t Loop::get_output() {
  return (int32_t)(counter-1);
}
