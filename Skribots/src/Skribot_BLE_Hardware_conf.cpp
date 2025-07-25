#include "Skribot_main.h"
#define DEBUG_MODE_H
#define SMART_ROTOR
#ifdef DEBUG_MODE_H
  #define DEBUG_PRINT(msg) Serial.println(msg)
#else
  #define DEBUG_PRINT(msg)
#endif

#define CASE_MATRIX     'M'
#define CASE_BUTTON     'P'
#define CASE_SROTOR     'R'
#define CASE_ROTOR      'Q'
#define CASE_DISTANCE   'D'
#define CASE_CLAW       'C'
#define CASE_LINE       'L'
#define CASE_LED        'W'
#define CASE_BUZZER     'B'
#define CASE_DHT11      'T'
#define CASE_ABS_DIV    'A'
#define I2C_DEV         'I'
#define SPI_DEV         'S'
byte Skribot::getPinN_for_hardware(char id){
        switch(id){
          case CASE_MATRIX:
            return(4);
          break;
          case SPI_DEV:
            return(4);
          break;
          case CASE_SROTOR:
            return(3);
          break;
          case CASE_ROTOR:
            return(2);
          break;
          case CASE_DISTANCE:
            return(2);
          break;
          case CASE_CLAW:
            return(2);
          break;
          case I2C_DEV:
            return(2);
          break;
          case CASE_ABS_DIV:
            return(2);
          default:
            return(1);
          break;
        }
  return(1);
}


bool Skribot::AddHardware(byte *tag){
  bool tmp = true;
  if(predefined_hrdw_config != "RAW_SKRIBRAIN") return(true); //ignoring BLE configuration when there is a user specified one
  switch(tag[0]){
    case CASE_MATRIX:
      DEBUG_PRINT("MATRIX");
      tmp = Add_Mono_LED_matrix(tag[2],tag[3],tag[4],tag[5],tag[1]);
    break;
    case CASE_BUTTON:
      
    
    break;
    case CASE_SROTOR:
    #ifdef SMART_ROTOR
      DEBUG_PRINT("SMOTOR");
      switch(tag[1])
      {
        case 1:
          DEBUG_PRINT("creating left smart rotor");
          leftSmartRotor = new SmartRotor(
            tag[2],
            tag[3],
            tag[4]
          );
          leftSmartRotor->begin();
          break;

        case 2:
          DEBUG_PRINT("creating right smart rotor");
          rightSmartRotor = new SmartRotor(
            tag[2],
            tag[3],
            tag[4]
          );
          rightSmartRotor->begin();
          break;
        default:
          break;
      }

      if (
        leftSmartRotor != nullptr &&
        rightSmartRotor != nullptr &&
        smartRotor == nullptr
      ) {
        DEBUG_PRINT("creating smart rotor system");
        smartRotor = new SmartRotorSystem(leftSmartRotor, rightSmartRotor);
        smartRotor->begin();
      } 
    #endif
      break;
      case CASE_ROTOR:
      DEBUG_PRINT("ROTOR");
        switch(tag[1]){
          case 1:
              AddDCRotor(tag[3],tag[2],"Left");
          break;
          case 2:
              AddDCRotor(tag[3],tag[2],"Right");
          break;
          default:
          break;
          }
      break;
    case CASE_DISTANCE:
        DEBUG_PRINT("DISTANCE");
          AddDistSensor(tag[2],tag[3],tag[1]);
        break;
    case CASE_BUZZER:
        DEBUG_PRINT("BUZZER");
          AddBuzzer(tag[2],tag[1]);
    break;
    case CASE_CLAW:
        DEBUG_PRINT("CLAW");
          AddClaw(tag[3],tag[2],tag[1]);
    break;
    case CASE_LINE:
        DEBUG_PRINT("LINE");
        AddLineSensor(tag[2], tag[1]);
    break;
    case CASE_LED:
        DEBUG_PRINT("LED");
        AddLED(tag[2],tag[1]);
    break;
    case SPI_DEV:
        DEBUG_PRINT("SPI_DEV");
        tmp = AddSPIDevice(tag[2],tag[3],tag[4],tag[5],tag[1]);
    break;
    case I2C_DEV:
        DEBUG_PRINT("I2C_DEV");
        tmp = AddI2CDevice(tag[2],tag[3],tag[4],tag[1]);
    break;
    case CASE_ABS_DIV:
        DEBUG_PRINT("ABS_DEV");
        tmp = AddAbstractDevice(tag[2],tag[3],tag[1]);
    break;
    case CASE_DHT11:
        DEBUG_PRINT("TEMP");
        AddDHT11Sensor(tag[2],tag[1]);
    break;
  }
  return(tmp);
}


void Skribot::ClearHardware(){
  DEBUG_PRINT("CLEAR HDW");
  if(predefined_hrdw_config != "RAW_SKRIBRAIN")return;
  if(SPIcomm[0] != NULL)delete SPIcomm[0];
  if(SPIcomm[1] != NULL)delete SPIcomm[1];
  if(I2Ccomm[0] != NULL)delete I2Ccomm[0];
  if(I2Ccomm[1] != NULL)delete I2Ccomm[1];

  for(byte tt = 0; tt < MAX_HARDWARE; tt++){
    if(tt < MAX_LED_MATRIX      && LED_Matrixes[tt]       !=NULL) delete LED_Matrixes[tt]       ;
    if(tt < MAX_LEFT_ROTORS     && LeftDCRotors[tt]       !=NULL) delete LeftDCRotors[tt]       ;
    if(tt < MAX_RIGHT_ROTORS    && RightDCRotors[tt]      !=NULL) delete RightDCRotors[tt]      ;
    if(tt < MAX_DIST_SENSORS    && DistSensors[tt]        !=NULL) delete DistSensors[tt]        ;
    if(tt < MAX_LINE_SENSORS    && LineSensors[tt]        !=NULL) delete LineSensors[tt]        ;
    if(tt < MAX_LEDs            && LEDs[tt]               !=NULL) delete LEDs[tt]               ;
    if(tt < MAX_CLAWS           && Claws[tt]              !=NULL) delete Claws[tt]              ;
    if(tt < MAX_BUZZERS         && Buzzers[tt]            !=NULL) delete Buzzers[tt]            ;
    if(tt < MAX_ABS_DIV         && Devices[tt]            !=NULL) delete Devices[tt]            ;
    if(tt < MAX_TEMP_SENSORS    && TemperatureSensors[tt] !=NULL) delete TemperatureSensors[tt] ;
  }


  SPIcomm[0] = NULL;
  SPIcomm[1] = NULL;
  I2Ccomm[0] = NULL;
  I2Ccomm[1] = NULL;

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

  NDistSensors    = 0;
  NLEDs           = 0;
  NLineSensors    = 0;
  NLeftDCRotors   = 0;
  NRightDCRotors  = 0;
  NClaws          = 0;
  NABSDevices     = 0;
  NSPIDevices     = 0;
  NI2CDevices     = 0;
  clearPWM();

  if (smartRotor != nullptr)
  {
    DEBUG_PRINT("deleting smart rotor system");
    delete smartRotor;
    smartRotor = nullptr;
    leftSmartRotor = nullptr;
    rightSmartRotor = nullptr;
  }

  if (leftSmartRotor != nullptr)
  {
    DEBUG_PRINT("deleting left smart rotor");
    delete leftSmartRotor;
    leftSmartRotor = nullptr;
  }

  if (rightSmartRotor != nullptr)
  {
    DEBUG_PRINT("deleting right smart rotor");
    delete rightSmartRotor;
    rightSmartRotor = nullptr;
  }

  hardware_checksum = 0;
  Serial.println("DELETED");
}



#undef DEBUG_PRINT
