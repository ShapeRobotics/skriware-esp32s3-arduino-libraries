#ifndef SKRIBOT_H
#define SKRIBOT_H

//#define DEBUG_MODE
//#define DEBUG_MODE_1

#define BOARD_VERSION 4
//#define FORCE_BOARD_VERSION_1
//#define FORCE_BOARD_VERSION_2
//#define FORCE_BOARD_VERSION_3
//#define FORCE_BOARD_VERSION_4

//#include "Arduino.h"
#ifndef _VARIANT_BBC_MICROBIT_
#include "EEPROM.h"
#include "./utilities/Claw.h"
#include "./utilities/Scope.h" 
#include "./utilities/RobotLED.h"
#include "./utilities/RGB_LED_Matrix.h"
#include "./utilities/BLEModule.h"
#include "./utilities/SPIHandler.h"
#include "./utilities/I2CHandler.h"
#include "./utilities/Mono_LED_Matrix.h"
#include "./utilities/Buzzer.h"
#include "./utilities/Buttons.h"
#include "./utilities/SmartRotorSystem.h"
#include "./utilities/Abstract_Device.h"
#include "./utilities/Gyroscope.h"
#include "./dht11/dht11.h"
#endif

#ifdef _VARIANT_BBC_MICROBIT_
#include "./utilities/Adafruit_Microbit.h"
#endif

#include "./utilities/DistSensor.h"
#include "./utilities/LineSensor.h"
#include "./utilities/Rotor.h"
#include "./utilities/LightSensor.h"
#include "./utilities/SoundDetector.h"
#include "./utilities/PWM_Handler.h"
#include "./utilities/StatusLED.h"
#include "./utilities/Speaker.h"


#ifndef ESP_H

#define BOARD_VERSION 1

#define EDU_ROTOR_SPEED_PIN_R 6
#define EDU_ROTOR_DIR_PIN_R 7
#define EDU_ROTOR_SPEED_PIN_L 5
#define EDU_ROTOR_DIR_PIN_L 4
#define EDU_ECHO_PIN_1 10
#define EDU_ECHO_PIN_2 12
#define EDU_TRIG_PIN_1 9
#define EDU_TRIG_PIN_2 11
#define EDU_LINE_SENSOR_1 A1
#define EDU_LINE_SENSOR_2 A2
#define EDU_LINE_SENSOR_3 A3
#define EDU_LED_DATA_PIN 0 
#define EDU_LED_DATA_PIN_1 1
#define EDU_CLAW_PIN1 2
#define EDU_CLAW_PIN2 8
#define EDU_SHIELD
#define EDU_SHIELD_STATUS_LED_R_PIN A6
#define EDU_SHIELD_STATUS_LED_G_PIN A5
#define EDU_SHIELD_STATUS_LED_B_PIN A4
#define EDU_SHILED_POWER_READOUT_PIN A7

#define LINE_PIN_1 EDU_LINE_SENSOR_1
#define LINE_PIN_2 EDU_LINE_SENSOR_2
#define LINE_PIN_3 EDU_LINE_SENSOR_3

#define L1 A1
#define L2 A2
#define L3 A3

#else

#define SKRIBRAIN_MOTOR_L_DIR1_PIN 12 
#define SKRIBRAIN_MOTOR_L_DIR2_PIN 21
#define SKRIBRAIN_MOTOR_L_ENC_PIN 35
#define SKRIBRAIN_MOTOR_R_DIR1_PIN 23
#define SKRIBRAIN_MOTOR_R_DIR2_PIN 22
#define SKRIBRAIN_MOTOR_R_ENC_PIN 32
#define SKRIBRAIN_ECHO_PIN_1 5
#define SKRIBRAIN_ECHO_PIN_2 19
#define SKRIBRAIN_TRIG_PIN_1 17
#define SKRIBRAIN_TRIG_PIN_2 18
#define SKRIBRAIN_SERVO_PIN_1 13
#define SKRIBRAIN_SERVO_PIN_2 4
#define SKRIBRAIN_SERVO_PIN_3 16
#define SKRIBRAIN_LED_PIN_1 2
#define SKRIBRAIN_LED_PIN_2 15
#define SKRIBRAIN_ANALOG_PIN_1 36  
#define SKRIBRAIN_ANALOG_PIN_2 39 
#define SKRIBRAIN_ANALOG_PIN_3 34
#define SKRIBRAIN_STATUS_LED_PIN 14

#define D1_PORT 1
#define D2_PORT 2

#define SERVO_1 0
#define SERVO_2 1

#define L1_PORT 1
#define L2_PORT 2
#define L3_PORT 3

#define LINE_PIN_1 SKRIBRAIN_ANALOG_PIN_1
#define LINE_PIN_2 SKRIBRAIN_ANALOG_PIN_2
#define LINE_PIN_3 SKRIBRAIN_ANALOG_PIN_3



#define SKRIBRAIN_2_MOTOR_L_DIR1_PIN 13 
#define SKRIBRAIN_2_MOTOR_L_DIR2_PIN 16
#define SKRIBRAIN_2_MOTOR_L_ENC_PIN 35
#define SKRIBRAIN_2_MOTOR_R_DIR1_PIN 12
#define SKRIBRAIN_2_MOTOR_R_DIR2_PIN 27
#define SKRIBRAIN_2_MOTOR_R_ENC_PIN 32
#define SKRIBRAIN_2_ECHO_PIN_1 26
#define SKRIBRAIN_2_ECHO_PIN_2 17
#define SKRIBRAIN_2_TRIG_PIN_1 25
#define SKRIBRAIN_2_TRIG_PIN_2 18
#define SKRIBRAIN_2_SERVO_PIN_1 8
#define SKRIBRAIN_2_SERVO_PIN_2 5
#define SKRIBRAIN_2_LED_PIN_1 15
#define SKRIBRAIN_2_LED_PIN_2 2
#define SKRIBRAIN_2_ANALOG_PIN_1 33  
#define SKRIBRAIN_2_ANALOG_PIN_2 39 
#define SKRIBRAIN_2_ANALOG_PIN_3 36
#define SKRIBRAIN_2_STATUS_LED_PIN 14

//#define SKRIBRAIN_2
#ifdef SKRIBRAIN_2

#define LINE_PIN_SK2_1 SKRIBRAIN_2_ANALOG_PIN_1
#define LINE_PIN_SK2_2 SKRIBRAIN_2_ANALOG_PIN_2
#define LINE_PIN_SK2_3 SKRIBRAIN_2_ANALOG_PIN_3
#endif

#endif

#define EEPROM_BOARD_VERSION_ADDR 5
#define EEPROM_SETTINGS_OVERRIDED_ADDR 6
#define EEPROM_LEFT_INVERT_ADDR 7
#define EEPROM_RIGHT_INVERT_ADDR 8
#define EEPROM_L1_BORDER_ADDR 9                         //16 bit values stored in two 8 bit addreses(ADDR and ADDR+1)
#define EEPROM_L2_BORDER_ADDR 11
#define EEPROM_L3_BORDER_ADDR 13
#define EEPROM_RIGHT_SCALE_ADDR 15
#define EEPROM_LEFT_SCALE_ADDR 16
                                                       //OLD EEPROM CONFIG         
#define EEPROM_LINE_DATA_START 17



#ifdef _VARIANT_BBC_MICROBIT_
#define SKRIBOT_MINI_SHILELD_MOTOR_1_DIR1_PIN 13 
#define SKRIBOT_MINI_SHILELD_MOTOR_1_DIR2_PIN 14
#define SKRIBOT_MINI_SHILELD_MOTOR_2_DIR1_PIN 15
#define SKRIBOT_MINI_SHILELD_MOTOR_2_DIR2_PIN 16
#define SKRIBOT_MINI_SHILELD_SENSOR_0 0
#define SKRIBOT_MINI_SHILELD_SENSOR_1 1
#define SKRIBOT_MINI_SHILELD_SENSOR_2 2
#define SKRIBOT_MINI_SHILELD_IR_IN 11
#define SKRIBOT_MINI_SHILELD_IR_OUT 9
#endif

#define LEFT_side 0
#define RIGHT_side 1

#define EEPROM_EMPTY_ESP32 255
#define EEPROM_EMPTY_ARDUINO 0

#define CALIB_MOTORS 0
#define CALIB_LINE_SENSORS 1

#define MAX_HARDWARE     5
#define MAX_DIST_SENSORS 5
#define MAX_LINE_SENSORS 5
#define MAX_CLAWS        2
#define MAX_LEFT_ROTORS  2
#define MAX_RIGHT_ROTORS 2
#define MAX_LEDs         5
#define MAX_LED_MATRIX   2
#define MAX_BUZZERS      5
#define MAX_BUTTONS      5
#define MAX_ABS_DIV      16
#define MAX_GYRO         2
#define MAX_TEMP_SENSORS 16

  class Skribot
 {
  public:
  	Skribot(String predef = " ");
  	void AddDistSensor(int EchoPin,int TrigPin,String Name);
    void AddDistSensor(int EchoPin,int TrigPin,int id);
    void AddDistSensor(byte DIST_PORT);

    void AddLineSensor(int Pin, String Name); 
    void AddLineSensor(int Pin, int id);
    void AddDCRotor(int SpeedPin,int DirectionPin, String side);

    void AddDHT11Sensor(int _pin,int id);

    bool AddI2CDevice(byte _SDA_PIN,byte _CLK_PIN,byte addr,byte id,uint32_t freq = 100000);
    bool AddSPIDevice(uint8_t MOSI_PIN,uint8_t MISO,uint8_t CLK_PIN,uint8_t CS_PIN,byte id);

     #ifndef _VARIANT_BBC_MICROBIT_ 
    void AddClaw(int ClawPin,int Arm_Pin, byte id = 0);
    void AddScope(int EchoPin,int Trigg,int ServoPin,String Name);
    void AddLED(int Pin,String name,byte N_LED = 1);
    void AddLED(int Pin, int id, byte N_LED  = 1);

    bool Add_Mono_LED_matrix(byte id){ return(true);};
    bool Add_Mono_LED_matrix(uint8_t MOSI_PIN,uint8_t MISO_PIN,uint8_t CLK_PIN,uint8_t CS_PIN,uint8_t id);
    void AddBuzzer(byte pin,byte id);
    void AddButton(byte pin,byte id);
    bool AddAbstractDevice(byte pin,byte device_type, byte device_id);

    bool AddGyroscope(byte _SDA_PIN=26, byte _CLK_PIN=27,byte id =0);

    void ConfigureSPIHandler(byte SPI_PORT);
    bool EEPROM_EMPTY(int val);
    #endif
    
                                                                  //functions for element adding
    void AddDistSensor(String SHIELD_SLOT);
    void AddLED(String SHIELD_SLOT);
    void AddLineSensor(String SHIELD_SLOT);

    void AddDCRotor(String SHIELD_SLOT);
    void AddClaw();  
                                                 //functions for elements adding when using Skriware shields
    void Set_Line_Sensor_Logic_Border(String id, int line, int noline);
    void wait_And_Check_BLE_Connection(int ms,int interval);

   

    void Move(char Direction,int ms);
    void FaceLeft(int ms = -1);
    void FaceRight(int ms = -1);
    void TurnLeft(int ms = -1);
    void TurnRight(int ms = -1);
    void MoveForward(int ms = -1);
    void MoveBack(int ms = -1);
    void RawRotorMove(int left,int right);

    void Set_Motor_Movment(byte motor_id, byte dir, byte speed,int time = -1);
    void Move_Motor_Forward(byte motor_id,byte speed,int time = -1);
    void Move_Motor_Back(byte motor_id,byte speed,int time = -1);
    void Stop();   
    void Invert_Left_Rotors(bool inv = true);
    void Invert_Right_Rotors(bool inv = true);
    
    void Scale_Left_Rotors(byte scale);
    void Scale_Right_Rotors(byte scale);

    void SetMotorSpeed(int speed);
    void SetSpeed_Raw(int speed);  
    void SetSpeed(int speed);                                                                    //functions for movements

    int ReadDistSensor(String id, int max = 150);
    int ReadDistSensor(int id, int max = 150);

    uint32_t performDeviceAction(byte device_id,byte input);
    
    void Configure_Connections(String predef="");

                                                                                                //distance sensor readout
    int ReadLineSensor(String name);
    int ReadLineSensor(int id);
    int ReadLineSensorData(int id);
    int ReadLineSensorData(String id);
                                                                                              // line sensor readout

    int ReadTemperature(int sensor_id);
    int ReadHumidity(int sensor_id);
                                                                                              // temperature and humidity readout
    Buzzer* GetBuzzer(int id);
                                                                                              //interface for buzzer functions 

    bool ConfigureBoardEEPROM();
    bool LoadCalibrationData();
    bool Check_Board_Version();
    int  Read_EEPROM_INT(byte addr);
    void Write_EEPROM_INT(byte addr,int val);
    
    void ReadIdFromCover();


    void Calibrate_sensors_no_Line();
    void Calibrate_sensors_Line();
    void Save_Calibration_Data(byte data_id);
    void SaveLineCalibrationValue(int ii);
    void Set_Line_Sensor_Logic_Border(LineSensor *tmp);

    void CloseClaw(byte id = 0);
    void OpenClaw(byte id  = 0);
    void Pick_Up(byte id = 0);
    void Put_Down(byte id = 0);                                                                //Claw functins

    void _CloseClaw(byte id = 0);
    void _OpenClaw(byte id  = 0);
    void _Pick_Up(byte id = 0);
    void _Put_Down(byte id = 0);                                                                //Claw functins
    
    void TurnLEDOn(int R,int G, int B,String name,byte N_LED =1);
    void TurnLEDOff(String name,byte N_LED =1); 
    void TurnLEDOn(int R,int G, int B,int _id = 0,byte N_LED = 1);
    void TurnLEDOff(int _id = 0,byte N_LED = 1);
    void SetLEDColor(int color_id);                                                            // LED functions

    int  LightSensor_Raw(int id);
    bool LightSensor_Dark(int id);
    bool LightSensor_Bright(int id);
                                                                                               //Light Sensor Functions
    void Set_gyroscope_calibration_parameters(float _X_minVal,float _X_maxval,float _Y_minVal,float _Y_maxval,float _Z_minVal,float _Z_maxval,byte gyro_id = 0);
    void Start_gyroscope_calibration(byte gyro_id =0);
    int  Gyroscope_read_X(byte gyro_id = 0);
    int  Gyroscope_read_Y(byte gyro_id = 0);
    int  Gyroscope_read_Z(byte gyro_id = 0);
    //BLE
    char BLE_read();                                                        
    void BLE_write(char *msg);
    void BLE_write(char const *msg);
    bool BLE_checkConnection();
    int  BLE_dataAvailable();
    void BLE_changeName(char name[],bool userConncection = true);
    void BLE_Setup();
    void BLE_reset();
    void BLE_Set_Module(moduleType type);
    void BLE_Flush();
    void BLE_Flush_Line();
    void sendNameInfo();
    bool ProgramENDRepotred();
    #ifdef ESP_H
    void SendMessage(int info);
    void SendMessage(char *info);
    void SendMessage(double info);
    void SendMessage(float info);

    void SendMessageLine(int info);
    void SendMessageLine(char *info);

    void SendMessageLine(double info);
    void SendMessageLine(float info);
    #endif

    void CONBRK();
    void IgnoreCONBRK();
    
    int BaterryCheck();
    byte ReadBattery();

    void EnterConfigMode();
    void ExitConfigMode();

    void AddHardware(char *tag);
    bool AddHardware(byte *tag);
    void ClearHardware();
    byte getPinN_for_hardware(char id);
    byte getIndexOfDeviceWithPin(byte pin);
    byte getPinOfDeviceWithID(byte id);
    byte getIndexOfDeviceWithID(byte id);

    void DEBUG_PRINT(char* msg);
 // private:
  DistSensor *DistSensors[MAX_DIST_SENSORS] = {NULL};
  LineSensor *LineSensors[MAX_LINE_SENSORS] = {NULL};
  Rotor *LeftDCRotors[MAX_LEFT_ROTORS] = {NULL};
  Rotor *RightDCRotors[MAX_RIGHT_ROTORS] = {NULL};
  RobotLED *LEDs[MAX_LEDs] = {NULL};
  Claw *Claws[MAX_CLAWS] = {NULL};
  Mono_LED_Matrix *LED_Matrixes[MAX_LED_MATRIX] = {NULL};
  Buzzer *Buzzers[MAX_BUZZERS] = {NULL};
  Abstract_Device *Devices[MAX_ABS_DIV] = {NULL};
  Gyroscope *gyroscopes[MAX_GYRO] = {NULL};
  dht11 *TemperatureSensors[MAX_TEMP_SENSORS] = {NULL};
  byte  NDistSensors,
        NBuzzers,
        NLEDs,NLineSensors,
        NLeftDCRotors,
        NRightDCRotors,
        NClaws,
        NDTH11,
        NABSDevices,
        NSPIDevices,
        NI2CDevices;           //counters
  byte TopId;

  StatusLED *status;
  SPIHandler *SPIcomm[2];
  I2CHandler *I2Ccomm[2];

  SmartRotor *leftSmartRotor = nullptr;
  SmartRotor *rightSmartRotor = nullptr;
  SmartRotorSystem *smartRotor = nullptr;

  bool using_BLE_Connection,
       connection_Break_Reported,
       program_End_Reported,
       stausLEDused,
       high_power_usage,
       claw_closed,
       config_mode,
       user_config,
       Remote_block_used,
       ignore_connection_break,
       eeprom_version_loaded;
  long claw_closed_time,hardware_checksum,claw_update_time;
  String predefined_hrdw_config;

#ifndef _VARIANT_BBC_MICROBIT_
  BLEModule *BTmodule;
#endif
moduleType BLE_MODULE_TYPE;

byte Board_type = 0;

//calibration parameters:
byte left_invert = 0;
byte right_invert = 0;
byte left_scale = 100;
byte right_scale = 100;
int L1_b = 1900;
int L2_b = 1900;
int L3_b = 1900;
byte calibraited_sensors_in_mem =  0;
int calibration_values[MAX_ABS_DIV];
uint8_t pins_for_calibration[MAX_ABS_DIV];

int DCSpeed = 140;
  
#ifdef _VARIANT_BBC_MICROBIT_
  Adafruit_Microbit_Matrix ledMatrix;
  Adafruit_Microbit_BLESerial BTLESerial;
#endif

};

 #endif
