#include "Skribot_main.h"



bool Skribot::EEPROM_EMPTY(int val){
    return(val == EEPROM_EMPTY_ESP32 || val == EEPROM_EMPTY_ARDUINO);
  }

bool Skribot::Check_Board_Version(){
      if(eeprom_version_loaded)return(true);
      #ifdef DEBUG_MODE
          Serial.println("Checking EEPROM...");
      #endif
       #ifdef ESP_H
       if(!EEPROM.begin(64)){
          #ifdef DEBUG_MODE
          Serial.println("EEPROM init fail, aborting EEPROM check.");
          #endif
          return(false);
       }
      #endif
       Board_type = EEPROM.read(EEPROM_BOARD_VERSION_ADDR);
       if (EEPROM_EMPTY(Board_type)) {                                   //No board Version defined
        EEPROM.write(EEPROM_BOARD_VERSION_ADDR,BOARD_VERSION);
        Board_type = BOARD_VERSION;                             //Asigning Board Version to the newest one.
         Serial.println(Board_type);
         #ifdef ESP_H 
              EEPROM.commit(); 
         #endif 
        #ifdef DEBUG_MODE
          Serial.println("First time flash detected");
        #endif
        return(false);
      }else{
        #ifdef FORCE_BOARD_VERSION_1
          Board_type = 1;
        #endif
        #ifdef FORCE_BOARD_VERSION_2
          Board_type = 2;
        #endif
        #ifdef FORCE_BOARD_VERSION_3
          Board_type = 3;
        #endif
        #ifdef FORCE_BOARD_VERSION_4
          Board_type = 4;
        #endif

        EEPROM.write(EEPROM_BOARD_VERSION_ADDR,Board_type);
        #ifdef ESP_H 
          EEPROM.commit(); 
        #endif
        #ifdef DEBUG_MODE
        Serial.print("BOARD VERSION:");
        Serial.println(Board_type);
        #endif
         
        eeprom_version_loaded = true;
        return(true);
      }
  }


  bool Skribot::LoadCalibrationData(){
    return(ConfigureBoardEEPROM());
  }

  bool Skribot::ConfigureBoardEEPROM(){
       if(!EEPROM.begin(64)){
          #ifdef DEBUG_MODE
          Serial.println("EEPROM init fail, aborting EEPROM check.");
          #endif
          return(false);
       }
       delay(10);                                              //EEPROM delay in order to avoid EEPROM ERRORS
       byte userChange = EEPROM.read(EEPROM_SETTINGS_OVERRIDED_ADDR);
       delay(10);
       if(EEPROM_EMPTY(userChange)){
         #ifdef DEBUG_MODE
          Serial.println("No user Settings in EEPROM Configuration");
        #endif
          user_config = false;
        return(false);                                                                         //No user change done aborting the process;
       }else if(userChange == 1){
       user_config = true;
       delay(10);                                              //EEPROM delay in order to avoid EEPROM ERRORS
       left_invert = EEPROM.read(EEPROM_LEFT_INVERT_ADDR);
       delay(10);                                              //EEPROM delay in order to avoid EEPROM ERRORS
       right_invert = EEPROM.read(EEPROM_RIGHT_INVERT_ADDR);
       delay(10);                                              //EEPROM delay in order to avoid EEPROM ERRORS
       left_scale = EEPROM.read(EEPROM_LEFT_SCALE_ADDR);
       delay(10); 
       right_scale = EEPROM.read(EEPROM_RIGHT_SCALE_ADDR);
       delay(10);                                             //EEPROM delay in order to avoid EEPROM ERRORS
       calibraited_sensors_in_mem = EEPROM.read(EEPROM_LINE_DATA_START);
       delay(10);
       
       if (EEPROM_EMPTY(calibraited_sensors_in_mem)) calibraited_sensors_in_mem = 0;            //user Line sensor calibration
       Serial.println("Calibration config:");
       for (int yy = 0; yy < calibraited_sensors_in_mem;yy++) {
           pins_for_calibration[yy] = EEPROM.read(EEPROM_LINE_DATA_START + 1 + 3 * yy);
           Serial.print(pins_for_calibration[yy]);
           Serial.print("->");
           delay(10);
           calibration_values[yy] = Read_EEPROM_INT(EEPROM_LINE_DATA_START + 2 + 3 * yy);
           Serial.println(calibration_values[yy]);
           delay(10);
       }
       }else{
        #ifdef DEBUG_MODE
        Serial.println("Unexpected user cofig value:");
        Serial.println(userChange);
        #endif
       }
       return(true);
   
  }

  void Skribot::Write_EEPROM_INT(byte addr,int value){
      EEPROM.write(addr,value);
      #ifdef ESP_H 
 EEPROM.commit(); 
 #endif
      delay(100);
      EEPROM.write(addr+1,value>>8);
      #ifdef ESP_H 
 EEPROM.commit(); 
 #endif
  }

  int Skribot::Read_EEPROM_INT(byte addr){
        int b3 = 0;
        int b1 =  EEPROM.read(addr+1);
        delay(10);
        int b2  = EEPROM.read(addr);
      if(EEPROM_EMPTY(b1)&& EEPROM_EMPTY(b2)){
        b3 = 0;
      }else{
        b3 = b2 | (int(b1) << 8);
      }
    return(b3);
  }


    void Skribot::Invert_Left_Rotors(bool inv){
       for(int kk = 0; kk < NLeftDCRotors ; kk++){
                    LeftDCRotors[kk]->invert_rotor(inv);
                  }
    }
    
    void Skribot::Invert_Right_Rotors(bool inv){
       for(int zz = 0; zz < NRightDCRotors ; zz++){
                    RightDCRotors[zz]->invert_rotor(inv);
                  }
    }

  
     void Skribot::Scale_Left_Rotors(byte scale){
      Serial.println("Scalling rotors:");
      Serial.println(scale);
       for(int kkk = 0; kkk < NLeftDCRotors ; kkk++){
                    LeftDCRotors[kkk]->scale_speed(scale);
                  }
    }
    
    void Skribot::Scale_Right_Rotors(byte scale){
       for(int zzz = 0; zzz < NRightDCRotors ; zzz++){
                    RightDCRotors[zzz]->scale_speed(scale);
                  }
    }

    void Skribot::SaveLineCalibrationValue(int ii) {
        EEPROM.write(EEPROM_LINE_DATA_START, ii+1);
        EEPROM.write(EEPROM_LINE_DATA_START + 1 + ii * 3, pins_for_calibration[ii]);
        Write_EEPROM_INT(EEPROM_LINE_DATA_START + 2 + ii * 3, calibration_values[ii]);
        EEPROM.commit();
    }

    void Skribot::Save_Calibration_Data(byte data_id){
        switch(data_id){
        case CALIB_MOTORS:
          EEPROM.write(EEPROM_LEFT_INVERT_ADDR,left_invert);
          EEPROM.write(EEPROM_RIGHT_INVERT_ADDR,right_invert);
          EEPROM.write(EEPROM_LEFT_SCALE_ADDR,left_scale);
          EEPROM.write(EEPROM_RIGHT_SCALE_ADDR,right_scale);
          Serial.println("Saving Motor calibration!");
          EEPROM.commit(); 
        break;
        case CALIB_LINE_SENSORS:
            EEPROM.write(EEPROM_LINE_DATA_START, NLineSensors);
            calibraited_sensors_in_mem = NLineSensors;
            for (int ii = 0; ii < NLineSensors; ii++) {
                EEPROM.write(EEPROM_LINE_DATA_START +  1 + ii*3, LineSensors[ii]->GetSensorPin());
                Write_EEPROM_INT(EEPROM_LINE_DATA_START + 2 + ii*3, LineSensors[ii]->GetLogicBorder());
                pins_for_calibration[ii] = LineSensors[ii]->GetSensorPin();
                calibration_values[ii] = LineSensors[ii]->GetLogicBorder();
            }
          EEPROM.commit(); 
        break;
      }
      if(!user_config){
          EEPROM.write(EEPROM_SETTINGS_OVERRIDED_ADDR,1);
          user_config = true;
          #ifdef ESP_H 
          EEPROM.commit(); 
          #endif
        }
    }
