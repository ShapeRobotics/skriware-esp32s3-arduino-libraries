#include "Skribot_mobile.h"


Skribot_mobile::Skribot_mobile(){}

BlockHandler BH;
Skribot *_robot;
bool transmision_recieved = false;
bool BT_state =false;
bool Connection_Break = false;
char ascitmp;
char softVersion[] = "1.0.0";
long last_BT_check = 0;
bool LED_STATE = true;

void ENTER_TO_IDLE(){
  BH.clear();
  _robot->Stop();
}

void Blink(){
  if (millis()-last_BT_check > 500){
    if(LED_STATE){
      _robot->TurnLEDOn(255,255,255);
    }else{
      _robot->TurnLEDOff();
    }
      last_BT_check = millis();
      LED_STATE = !LED_STATE;
  }
}

int freeRam() 
{
  #ifndef ESP_H
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
  #else
  return(500);
  #endif
}


int CompileCode(){
     int flag; 
  while(freeRam() > 190){
      flag = BH.Handle_Msg(); 
      if(flag != CODE_NOT_COMPLETE)return(flag);
    }
    return(CODE_NOT_VALID);
}

void idle_connectioncheck(){
   if(!_robot->BLE_checkConnection()){
     if(BT_state){
      _robot->Stop();
      BT_state = !BT_state;
      Serial.println("DISCONNECTED");
      ENTER_TO_IDLE();
      _robot->BLE_reset();
     }
  }else if(_robot->BLE_checkConnection() && !BT_state){ 
     BT_state = !BT_state;
      Serial.println("CONNECTED");
     BH.clear();
  }
  if(_robot->BTmodule->OTAupdate){
    if(millis()-_robot->BTmodule->last_OTA_data_recive > 5000){
        Serial.println("ENDING OTA");
        Serial.println(millis()-_robot->BTmodule->last_OTA_data_recive);
      _robot->BTmodule->OTA_end();
      _robot->status->TurnOn(BLUE, 2);
      _robot->status->CheckBateryStatus(true);
    }
  }  
} 

void SendCodeEndMEssage(){
        BH.clear();
        //_robot->TurnLEDOn(255,255,255);
        if(!Connection_Break){
          _robot->ProgramENDRepotred();
          if(!_robot->Remote_block_used){
            _robot->wait_And_Check_BLE_Connection(200,20);
            _robot->BLE_write("DONE\n");
          }else{
            _robot->Remote_block_used = false;
          }
        }
        _robot->connection_Break_Reported = false;
        #ifdef DEBUG_MODE
          Serial.println("CONFIRMING END OF CODE");
        #endif
        ENTER_TO_IDLE();
}
void ExecuteCode(){
        #ifdef DEBUG_MODE
          Serial.println("CONFIRMING START OF CODE");
        #endif
        _robot->status->ReadBatteryState();
while(BH.doBlock()){
           if(_robot->ProgramENDRepotred()){
              #ifdef DEBUG_MODE
              Serial.println("Stopping the _robot!");
              #endif
                BH.clear();
                _robot->Stop();
              break;  
            }else if(!_robot->BLE_checkConnection()){
                ENTER_TO_IDLE();
                Connection_Break = true;
                break;
            }
        }
}

void SendErrorMsg(char *msg){
            ENTER_TO_IDLE();
            #ifdef ESP_H        
            _robot->status->TurnOn(RED,2);
            #endif
            #ifdef DEBUG_MODE
            Serial.println(msg);
            #endif
            Block::robot->BLE_Flush();
}

void SendErrorMsg(char const *msg){
            SendErrorMsg((char*)msg);
}

void skribot_setup() {
  pinMode(6,OUTPUT);
    _robot = new Skribot("RAW_SKRIBRAIN");
  _robot->BLE_Setup();
  Block::setRobot(_robot); 
  Block::setBlockHandler(&BH);
  BT_state = false; 
  #ifdef DEBUG_MODE
    Serial.println(freeRam());
  #endif 
    //UserFunctions_Setup();
    ENTER_TO_IDLE();
    Block::robot->ClearHardware();
    digitalWrite(6,LOW);
}

void skribot_loop() {
    byte tmp = BH.readMessageLine();
    BH.processMessageLine(tmp);
    
    if(BH.transfereBlocks){
      byte codeinfo;
      while(true){                  //read code
        codeinfo = BH.readCodeLine();
        if(codeinfo == TIMEOUT_ERROR_CODE)break;
        if(codeinfo == CODE_COMPLETE)break;
        if(codeinfo == CODE_TOO_LONG)break;
      }
      if(codeinfo == CODE_COMPLETE){
        Serial.println("BEGIN COMPILATION!");
        byte succes = CompileCode();        //make blocks connections
        Serial.println("COMPILATION ENDED");
        if(succes == CODE_VALID_AND_COMPLETE_RUN_O){                    //chceck compiler errors
            Connection_Break = false;
            if(!_robot->Remote_block_used)_robot->BLE_write("OK");
            ExecuteCode();                  //Here _robot runs the code
            SendCodeEndMEssage();

        }else if(succes == CODE_NOT_VALID){
          _robot->BLE_write("ERROR:CODE_NOT_VALID\n");
          SendErrorMsg("CODE NOT VALID");
        }else if(succes == CODE_TOO_LONG){
          _robot->BLE_write("ERROR:STOP:CODE_TOO_LONG\n");
          SendErrorMsg("CODE TOO LONG");
        }

      }else if(codeinfo == TIMEOUT_ERROR_CODE){
          SendErrorMsg("TIMEOUT ERROR");
          _robot->BLE_write("ERROR:TIMEOUT_ERROR\n");
      }else if(codeinfo == CODE_TOO_LONG){
          _robot->BLE_write("ERROR:STOP:CODE_TOO_LONG\n");
          SendErrorMsg("CODE TOO LONG");
      } 

  }
  idle_connectioncheck();
}

byte Board_Version(){
      byte Board_type = BOARD_VERSION;
      #ifdef DEBUG_MODE
          Serial.println("Checking EEPROM...");
      #endif
       #ifdef ESP_H
       if(!EEPROM.begin(64)){
          return(BOARD_VERSION);
       }
      #endif
       Board_type = EEPROM.read(EEPROM_BOARD_VERSION_ADDR);
       if(Board_type == EEPROM_EMPTY_ESP32 || Board_type == EEPROM_EMPTY_ARDUINO){                                   //No board Version defined
        EEPROM.write(EEPROM_BOARD_VERSION_ADDR,BOARD_VERSION);
        Board_type = BOARD_VERSION;                             //Asigning Board Version to the newest one.
         #ifdef ESP_H 
              EEPROM.commit(); 
         #endif 
      }else{
        #ifdef FORCE_BOARD_VERSION_1
          Board_type = 1;
          EEPROM.write(EEPROM_BOARD_VERSION_ADDR,Board_type);
          EEPROM.commit();
            #warning "V1"
        #endif
        #ifdef FORCE_BOARD_VERSION_2
          Board_type = 2;
          EEPROM.write(EEPROM_BOARD_VERSION_ADDR,Board_type);
          EEPROM.commit();
          #warning "V2"
        #endif
        #ifdef FORCE_BOARD_VERSION_3
          Board_type = 3;
          EEPROM.write(EEPROM_BOARD_VERSION_ADDR,Board_type);
          EEPROM.commit();
          #warning "V3"
        #endif
        #ifdef FORCE_BOARD_VERSION_4
          Board_type = 4;
          EEPROM.write(EEPROM_BOARD_VERSION_ADDR, Board_type);
          EEPROM.commit();
          #warning "V4"
        #endif
        
      }
      return(Board_type);
  }

  void raport_user_code_mode(byte Board_type){
        StatusLED *status = NULL;
        switch (Board_type){
          case 1:
            status = new StatusLED(SKRIBRAIN_STATUS_LED_PIN,SKRIBRAIN_SERVO_PIN_3,1);
          break;
          case 2:
            status = new StatusLED(SKRIBRAIN_STATUS_LED_PIN,SKRIBRAIN_SERVO_PIN_2,2);
          break;
          case 3:
            status = new StatusLED(SKRIBRAIN_STATUS_LED_PIN,SKRIBRAIN_MOTOR_L_ENC_PIN,3);
          break;
          case 4:
              status = new StatusLED(SKRIBRAIN_STATUS_LED_PIN, SKRIBRAIN_MOTOR_L_ENC_PIN, 4);
              break;
          default:
          break;
        }
        status->TurnOn(OFF,1);
        status->TurnOn(GREEN,2);
        delete status;
  }

  void reset_to_user_code(){
    ESP.restart();
}