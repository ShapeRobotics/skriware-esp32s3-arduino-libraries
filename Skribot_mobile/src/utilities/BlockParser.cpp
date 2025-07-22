	#include "BlockHandler.h"
  #include "../Skribot_mobile.h"


  int BlockHandler::portUID(byte portid){
        int primes[] = {2,3,5,7,11,13,17,19,23};
        return(primes[portid]);
  }

  bool BlockHandler::AddToMessage(char x){
    if(messageLength < MAX_MSG_L){
          AllMessage[messageLength] = x;
          messageLength++;
          #ifdef DEBUG_MODE
          Serial.print(x);
          #endif
          return (true);
    }
          return(false);
  }

  void BlockHandler::CheckLongCodes(char *asciTmp){
    if(*asciTmp == 'B'){                                                  //Baptised case 
          while(true){
            if(Block::robot->BLE_dataAvailable()){
              char tmp2 = Block::robot->BLE_read();
              if(tmp2 == 'A'){
                *asciTmp = 'b';
                break; 
              }else if(tmp2 == 'E'){
                break;
              }else{
                *asciTmp = INVALID_MSG_ERROR_CODE;
              }
            }
          }
        }else if(*asciTmp == 'R'){
          while(true){
            if(Block::robot->BLE_dataAvailable()){
              char tmp2 = Block::robot->BLE_read();
              tmp2 = Block::robot->BLE_read();
              if(tmp2 == 'S'){
                *asciTmp = 'r';
                break; 
              }else if(tmp2 == 'N'){
                break;
              }else{
                *asciTmp = INVALID_MSG_ERROR_CODE;
                break;
              }
            }
          }
        }
  }

  byte BlockHandler::read_complete_line(bool add_to_code,char _MainAsci){
        char asciTmp = _MainAsci;
        while(asciTmp != '\n'){
          if(Block::robot->BLE_dataAvailable()){
            asciTmp = Block::robot->BLE_read();
            if(add_to_code)AddToMessage(asciTmp); 
          }else{
            if(CheckForTimeout()){
              if(add_to_code)transfereBlocks = false;  
              return(TIMEOUT_ERROR_CODE);
            }
          }
        }
      return(_MainAsci);
  }

  bool BlockHandler::CheckForTimeout(){
            sei();
            bool tmp = false;
            long last_message_time = millis();
            long last_ack_send = last_message_time;
            delay(ack_resend_time);
            Block::robot->BLE_write("ack\n");
            while((Block::robot->BLE_dataAvailable() == 0)){
              if(millis() - last_message_time > MESSAGE_TIMEOUT){
                tmp = true;
                if(ack_resend_time < MAX_ACK_RESENT_TIME)ack_resend_time += AFTER_TIMOUT_DELAY_INCREASE;
                break;
              }
            }
            return(tmp);
  }

  byte BlockHandler::readMessageLine(){
      char MainAsci;
      if(Block::robot->BLE_dataAvailable()){
        MainAsci = Block::robot->BLE_read();                                 //Reading first character of the message 255-error Code
        CheckLongCodes(&MainAsci);
        if(MainAsci == INVALID_MSG_ERROR_CODE) return(INVALID_MSG_ERROR_CODE);
        #ifdef DEBUG_MODE_1
        Serial.print(MainAsci);
        #endif
        if(MainAsci != 'H' && MainAsci != 'C' && MainAsci != 'G'){
            MainAsci = read_complete_line(false,MainAsci);
        }
        return(MainAsci);
      }else{
        return(NO_MSG_CODE);
      }
  }
  byte BlockHandler::readCodeLine(){
    char MainAsci;
    if(Block::robot->BLE_dataAvailable()){
      MainAsci = Block::robot->BLE_read();                                 //Reading first character of the message 255-error Code
      AddToMessage(MainAsci);  
      MainAsci = read_complete_line(true,MainAsci);
      if(MainAsci == 'R'){
        transfereBlocks = false;
        return(CODE_COMPLETE);
      }
      if(MainAsci == TIMEOUT_ERROR_CODE)return(TIMEOUT_ERROR_CODE);
    }else{
      CheckForTimeout();
      return(NO_MSG_CODE);
    }
    if(!(messageLength < MAX_MSG_L))return(CODE_TOO_LONG);
    return(CODE_PASSED);
  }

  void BlockHandler::processMessageLine(byte LineCode){
        char tmp = 'A';
        char tmpNameArray[32] = {' '};
        int tmpCounter;
        char tmp_tag[10] = {' '};
        byte hardware_types[15][6] = {{' '},{' '}};
        int n_hardware = 0;
        bool vaildcommand = true;
        int tmp_checksum = 0;
        byte n_pins;
        int tmp_int[10] = { 0 };
        switch(LineCode){
          case BAPTISED:
                #ifdef DEBUG_MODE 
                  Serial.println("Named");
                #endif
                  Block::robot->sendNameInfo();
                  clear();
          break;
          case RENAME:
                #ifdef DEBUG_MODE 
                  Serial.println("NewName");
                #endif
                tmp = Block::robot->BLE_read();
                tmpNameArray[32] = {' '};
                tmpCounter = 0;
                while(tmp != '\n'){
                  if(Block::robot->BLE_dataAvailable()){
                    tmpNameArray[tmpCounter] = tmp;
                    tmpCounter++;
                    tmp = Block::robot->BLE_read();
                  }
                }
                #ifdef DEBUG_MODE 
                  Serial.println(tmpNameArray);
                #endif  
                Block::robot->BLE_changeName(tmpNameArray);
                clear();
          break;
          case VERSION:
                #ifdef DEBUG_MODE 
                  Serial.println("Version Request");
                #endif
                  if (Block::robot->Board_type == 4) {
                      sprintf(tmp_tag, "VERSION:%s%d", HARDWARE_VERSION, Block::robot->TopId);
                  }
                  else if (Block::robot->Board_type < 4) {
                      sprintf(tmp_tag, "VERSION:%s", "2.3.0");
                  }
                Block::robot->BLE_write(tmp_tag);
                clear();
          break;
          case F_VERSION:
               #ifdef DEBUG_MODE 
                  Serial.println("Firmware version Request");
                #endif
                sprintf(tmp_tag, "F_VERSION:%s", FIRMWARE_VERSION);
                Block::robot->BLE_write(tmp_tag);
                clear();
          break;
          case RESET:
              #ifdef DEBUG_MODE 
                Serial.println("RESET BLE");
              #endif
                Block::robot->BLE_reset();
                clear();
          break;
          case END:
                clear();
                Block::robot->Stop();
          break;
          case BEGIN:
                transfereBlocks = true;
          break;
          case NO_MSG_CODE:
          
          break;
          case REMOTE:
              if (Block::robot->Check_Board_Version() != 4 && Block::robot->NLeftDCRotors == 0 && Block::robot->NRightDCRotors == 0) {
                  Block::robot->AddDCRotor(SKRIBRAIN_MOTOR_L_DIR2_PIN, SKRIBRAIN_MOTOR_L_DIR1_PIN, "Left");
                  Block::robot->AddDCRotor(SKRIBRAIN_MOTOR_R_DIR2_PIN, SKRIBRAIN_MOTOR_R_DIR1_PIN, "Right");
              }
              Block::robot->RawRotorMove(readIntDirect(),readIntDirect());
          break;
          case GRIPPER:
              if (Block::robot->NClaws == 0 && Block::robot->Check_Board_Version() != 4)Block::robot->AddClaw();
            tmp = BLE_readwithTIMEOUT();
            tmp = BLE_readwithTIMEOUT();
          switch(tmp){
              case  '1':
             Block::robot->_CloseClaw();
            break;
               case '2':
             Block::robot->_OpenClaw();
            break;
               case '3':
             Block::robot->_Pick_Up();
            break;
               case '4':
             Block::robot->_Put_Down();
            break;
          }
            tmp = BLE_readwithTIMEOUT();
          break;
          case BATTERY:
          sprintf(tmp_tag,"%d",Block::robot->ReadBattery());
          Block::robot->BLE_write(tmp_tag);
          break;
          case PIANO:
              if(Block::robot->Buzzers[SERVO_2] == NULL)Block::robot->AddBuzzer(SKRIBRAIN_SERVO_PIN_2,SERVO_2);
              switch(readIntDirect()){
                      case 0:
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->StopNote();
                        break;
                      case 1:   
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("C4");
                        break;
                      case 2:
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("C#4");
                        break;
                      case 3: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("D4");
                        break;
                      case 4: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("D#4");
                        break;
                      case 5: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("E4");
                        break;
                      case 6: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("F4");
                        break;
                      case 7: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("F#4");
                        break;
                      case 8: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("G4");
                        break;
                      case 9: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("G#4");
                        break;
                      case 10: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("A4");
                        break;
                      case 11: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("A#4");
                        break;
                      case 12: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("B4");
                        break;
                      case 13: 
                        if (Block::robot->Buzzers[SERVO_2] != NULL)
                          Block::robot->Buzzers[SERVO_2]->PlayNote("C5");
                        break;
                      default:
                        break;
              }
          break;
          case HARDWARE_SET:
                tmp = BLE_readwithTIMEOUT();
                while(true){
                tmp = BLE_readwithTIMEOUT();

                if(tmp == '\n' || n_hardware == 15)break;
                if(tmp == ' ')tmp = BLE_readwithTIMEOUT();
                hardware_types[n_hardware][0] = (byte)tmp;
                n_pins = Block::robot->getPinN_for_hardware(hardware_types[n_hardware][0]);
                hardware_types[n_hardware][1] = (byte)readIntDirect();
                for(byte yy = 2; yy < n_pins+2;yy++){
                  hardware_types[n_hardware][yy] = readIntDirect();
                }
                tmp_checksum += hardware_types[n_hardware][0]*portUID(n_hardware);
                n_hardware++;
                }
              if(tmp_checksum != Block::robot->hardware_checksum || (Block::robot->Board_type == 4 && Block::robot->TopId == 3)){
                Block::robot->ClearHardware();
                for(byte tt = 0; tt< n_hardware+1;tt++)Block::robot->AddHardware(hardware_types[tt]);
                Block::robot->hardware_checksum = tmp_checksum;  
                Block::robot->DEBUG_PRINT((char*)"HARDWARE SET");
              }else{
                Block::robot->DEBUG_PRINT((char*)"Now Hardware changes.");
              }
          break;
          case CALIBRATE:
              tmp = BLE_readwithTIMEOUT();
              tmp_tag[0] = BLE_readwithTIMEOUT();
              tmp = BLE_readwithTIMEOUT();
              if(tmp_tag[0] == 'M'){
                Block::robot->left_scale= readIntDirect();
                Block::robot->right_scale= readIntDirect();
                Block::robot->left_invert= readIntDirect();                
                Block::robot->right_invert= readIntDirect();
                      #ifdef DEBUG_MODE
                        Serial.println("User Corrections:");
                        Serial.print("LS: ");
                        Serial.println(Block::robot->left_scale);
                        Serial.print("RS: ");
                        Serial.println(Block::robot->right_scale);
                        Serial.print("LI: ");
                        Serial.println(Block::robot->left_invert);
                        Serial.print("RI: ");
                        Serial.println(Block::robot->right_invert);
                      #endif
              Block::robot->Save_Calibration_Data(CALIB_MOTORS);
              Block::robot->Invert_Left_Rotors(Block::robot->left_invert);
              Block::robot->Scale_Left_Rotors(Block::robot->left_scale);
              Block::robot->Invert_Right_Rotors(Block::robot->right_invert);
              Block::robot->Scale_Right_Rotors(Block::robot->right_scale);
              }else if(tmp_tag[0] == 'W'){                                  //AutoCalibration wite read
                #ifdef DEBUG_MODE
              Serial.println("Calibrating No Line!");
               #endif
              Block::robot->Calibrate_sensors_no_Line();
              }else if(tmp_tag[0] == 'B'){                                  //Autocalibration black read
                 Serial.println("Calibrating Line!");
              Block::robot->Calibrate_sensors_Line();
              Block::robot->Save_Calibration_Data(CALIB_LINE_SENSORS);
              Block::robot->ClearHardware();
              }
              else if (tmp_tag[0] == 'Q') {
                  tmp_int[0] = readIntDirect();
                  tmp_int[1] = readIntDirect();
                  tmp_int[2] = readIntDirect();
                  tmp = BLE_readwithTIMEOUT();
                  Block::robot->pins_for_calibration[tmp_int[2]] = tmp_int[0];
                  Block::robot->calibration_values[tmp_int[2]] = tmp_int[1];
                  Block::robot->SaveLineCalibrationValue(tmp_int[2]);
                  Serial.println("Saving for pin");
                  Serial.println(tmp_int[0]);
                  Serial.println(tmp_int[1]);
              }
               if(tmp_tag[0] != 'Q')Block::robot->BLE_Flush();
          break;
          case ECHO_TEST:
            tmp = BLE_readwithTIMEOUT();
            tmp_int[0] = readIntDirect();
            sprintf(tmp_tag,"TEST:%d",tmp_int[0]);
            Block::robot->BLE_write(tmp_tag);
          break;
          case FIRMWARE_UPDATE:
            Block::robot->BTmodule->OTAupdate = true;
            Block::robot->BTmodule->OTA_start();
            Block::robot->BLE_write("READY FOR BINARY FILE\n");
            Block::robot->status->TurnOn(PURPLE,2);
            break;
          default:
              Block::robot->BLE_Flush();
              clear();
              vaildcommand = false;
          break;
        }

        if(LineCode == TIMEOUT_ERROR_CODE){
          Serial.println("Timeout Error");
          Block::robot->BLE_write("ERROR:TIMEOUT_ERROR\n");
          return;
        }
        if(LineCode != NO_MSG_CODE && !transfereBlocks && LineCode != HARDWARE_SET && LineCode != FIRMWARE_UPDATE){
        if(vaildcommand){
          sprintf(tmp_tag,"%cOK\n",LineCode);
          Block::robot->BLE_write(tmp_tag);
        }else{
          sprintf(tmp_tag,"ERROR:STOP:UNKNOWN_COMMAND:%c\n",LineCode);
          Block::robot->BLE_write(tmp_tag);
        }
      }
  } 

  char BlockHandler::BLE_readwithTIMEOUT(){
    char asciTmp;
    if(Block::robot->BLE_dataAvailable()){
            asciTmp = Block::robot->BLE_read();
          }else{
            if(CheckForTimeout()){
              return(TIMEOUT_ERROR_CODE);
            }else{
              asciTmp = Block::robot->BLE_read();
            }
          }
      return(asciTmp);
}

  int32_t BlockHandler::readIntDirect(byte max_d){
  int nDigits = 0;
  int sign = 1;
  byte cursor = 0;
  char tmp;
  char msg[10];
  tmp = BLE_readwithTIMEOUT();
  while(tmp != ' ' && tmp != '\n' && tmp < 250){
    msg[cursor+nDigits] = tmp;
    tmp = BLE_readwithTIMEOUT();
    nDigits++;
  }
  
  if(msg[cursor] == '-')sign = -1;
  if(sign==-1){
    cursor++;
    nDigits--;
  }
  int32_t out = 0;
  int32_t power = 1;
    for(int ii = nDigits-1; ii > -1; ii--){
      if(ii != nDigits-1) power *=10;
      int32_t add = cti(msg[cursor + ii])*power;
      out += add;
    }
    return(out*sign);
}



          