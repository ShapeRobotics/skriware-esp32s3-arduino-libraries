#include "Skribot_main.h"

#define EEPROM_EMPTY_ESP32 255
#define EEPROM_EMPTY_ARDUINO 0


Skribot::Skribot(String predef){
  //#ifdef DEBUG_MODE
  Serial.begin(115200);
  //#endif
  NDistSensors    = 0;
  NLEDs           = 0;
  NLineSensors    = 0;
  NLeftDCRotors   = 0;
  NRightDCRotors  = 0;
  NClaws          = 0;
  NABSDevices     = 0;
  NSPIDevices     = 0;
  NI2CDevices     = 0;
  BTmodule   = NULL;
  SPIcomm[0] = NULL;
  SPIcomm[1] = NULL;
  I2Ccomm[0] = NULL;
  I2Ccomm[1] = NULL;
  using_BLE_Connection = false;
  program_End_Reported = false;
  connection_Break_Reported = false;
  stausLEDused = false;
  ignore_connection_break = false;
  high_power_usage = false;
  eeprom_version_loaded = false;
  claw_closed = false;
  claw_closed_time = 0;
  config_mode = false;
  Remote_block_used = false;
  leftSmartRotor = NULL;
  rightSmartRotor = NULL;
  smartRotor = NULL;
  predefined_hrdw_config = " ";
  TopId  = 3;
  for(byte tt = 0; tt < MAX_HARDWARE; tt++){
    if(tt < MAX_LED_MATRIX)   LED_Matrixes[tt]  =NULL;
    if(tt < MAX_LEFT_ROTORS)  LeftDCRotors[tt]  =NULL;
    if(tt < MAX_RIGHT_ROTORS) RightDCRotors[tt] =NULL;
    if(tt < MAX_DIST_SENSORS) DistSensors[tt]   =NULL;
    if(tt < MAX_LINE_SENSORS) LineSensors[tt]   =NULL;
    if(tt < MAX_LEDs)         LEDs[tt]          =NULL;
    if(tt < MAX_CLAWS)        Claws[tt]         =NULL;
    if(tt < MAX_BUZZERS)      Buzzers[tt]       =NULL;
    if(tt < MAX_ABS_DIV)      Devices[tt]       =NULL;
  }

  if(predef != " ")Configure_Connections(predef);
}

  int Skribot::BaterryCheck(){
      int tmpStatus;
      if(stausLEDused && !high_power_usage){
        tmpStatus = status->CheckBateryStatus();
      }else{
        tmpStatus = 0;
      }
      return(tmpStatus);
    }

    void Skribot::EnterConfigMode(){
      config_mode = true;
    }
    void Skribot::ExitConfigMode(){
      if(config_mode){
        config_mode = false;
      }
    }

    void Skribot::Set_Line_Sensor_Logic_Border(LineSensor* tmp) {
        for (int zz = 0; zz < calibraited_sensors_in_mem; zz++) {
            if (tmp->GetSensorPin() == pins_for_calibration[zz]) {
                tmp->SetLogicBorder(calibration_values[zz]);
                DEBUG_PRINT((char*)"Setting pin:");
                Serial.println(pins_for_calibration[zz]);
                Serial.println(calibration_values[zz]);
            }
        }
    }
  void Skribot::Set_Line_Sensor_Logic_Border(String id, int line, int noline){
    int unscaledline = line/100*4095;
    int unscaledNoline = noline/100*4095;
    if(!EEPROM.begin(64)){
          #ifdef DEBUG_MODE
          Serial.println("EEPROM init fail, aborting calibration");
          #endif
          return;
       }
                        int lb = unscaledline +abs(unscaledline-unscaledNoline)/2;
                        if(id == "L1"){
                          Write_EEPROM_INT(EEPROM_L1_BORDER_ADDR,lb);
                          L1_b = lb;
                        }
                        if(id == "L2"){
                          Write_EEPROM_INT(EEPROM_L2_BORDER_ADDR,lb);
                          L2_b = lb;
                        }
                        if(id == "L3"){
                          Write_EEPROM_INT(EEPROM_L3_BORDER_ADDR,lb);
                          L3_b = lb;
                        }
                        #ifdef ESP_H 
                        EEPROM.commit(); 
                        #endif
                        delay(100);
                        if(!user_config){
                          EEPROM.write(EEPROM_SETTINGS_OVERRIDED_ADDR,1);
                          user_config = true;
                          #ifdef ESP_H 
                          EEPROM.commit(); 
                          #endif
                    
                        }
  Serial.print(id);
  Serial.println(" calibrated!");
}
byte Skribot::ReadBattery(){
  return(status->ReadBatteryState());
}

void Skribot::Calibrate_sensors_no_Line(){
                   for(int zz = 0; zz < NLineSensors ; zz++){
                      TurnLEDOn(255,255,255);
                      LineSensors[zz]->No_Line_Readout();
                      TurnLEDOn(0,0,0);
                      delay(100);
                    }
    }
void Skribot::Calibrate_sensors_Line(){
                    for(int zz = 0; zz < NLineSensors ; zz++){
                      TurnLEDOn(255,255,255);
                      LineSensors[zz]->Line_Readout();
                      TurnLEDOn(0,0,0);
                      delay(100);
                    }
    }
void Skribot::DEBUG_PRINT(char* msg) {
#ifdef DEBUG_MODE
    Serial.println(msg);
#endif

}
void Skribot::ReadIdFromCover(){
  #define load      22
  #define clockIn   23
  #define dataIn    21

  pinMode(load, OUTPUT);
  pinMode(dataIn, INPUT);
  pinMode(clockIn, OUTPUT);

  digitalWrite(load, LOW);
  delayMicroseconds(5);
  digitalWrite(load, HIGH);
  delayMicroseconds(5);

  digitalWrite(clockIn, HIGH);
  TopId = shiftIn(dataIn, clockIn, MSBFIRST);
}