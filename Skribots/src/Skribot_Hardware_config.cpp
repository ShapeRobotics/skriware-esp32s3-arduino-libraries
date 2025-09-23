#include "Skribot_main.h"

void Skribot::AddDCRotor(int SpeedPin,int DirectionPin, String Side){
    Rotor *dcrotor = new Rotor(SpeedPin,DirectionPin);
     if(Side == "Left"){
      LeftDCRotors[NLeftDCRotors] = dcrotor;
      NLeftDCRotors++;
      Serial.println("Rotor Added!");
      if(user_config){
        if(!EEPROM_EMPTY(left_invert))Invert_Left_Rotors(left_invert);
        if(!EEPROM_EMPTY(left_scale))Scale_Left_Rotors(left_scale);
      }
     }else if(Side == "Right"){
      RightDCRotors[NRightDCRotors] = dcrotor;
      NRightDCRotors++;
      Serial.println("Rotor Added!");
      if(user_config){
        if(!EEPROM_EMPTY(right_invert))Invert_Right_Rotors(right_invert);
        if(!EEPROM_EMPTY(right_scale))Scale_Right_Rotors(right_scale);
      }
     }
  }

  void Skribot::AddDistSensor(int EchoPin,int TrigPin,String name){
    DistSensor *dsensor = new DistSensor(EchoPin,TrigPin,name);
    DistSensors[NDistSensors] = dsensor;
    NDistSensors++;
  }

  void Skribot::AddDistSensor(int EchoPin,int TrigPin,int id){
    DistSensor *dsensor = new DistSensor(EchoPin,TrigPin,id);
    DistSensors[NDistSensors] = dsensor;
    NDistSensors++;
  }

  void Skribot::AddLineSensor(int pinL,String Name){
    LineSensor *lsensor = new LineSensor(pinL,Name);
    LineSensors[NLineSensors] =  lsensor;
    NLineSensors++;
    Set_Line_Sensor_Logic_Border(lsensor);
  }

  void Skribot::AddLineSensor(int pinL,int id){
    LineSensor *lsensor = new LineSensor(pinL,id);
    LineSensors[NLineSensors] =  lsensor;
    NLineSensors++;
    Set_Line_Sensor_Logic_Border(lsensor);
  }

  void Skribot::AddDHT11Sensor(int _pin,int id){
    dht11 *tempSens = new dht11(_pin,id);
    TemperatureSensors[NDTH11] = tempSens;
    NDTH11++;
  }

#ifndef _VARIANT_BBC_MICROBIT_
      
    void Skribot::AddDistSensor(String SHIELD_SLOT){
      #ifndef ESP_H 
      if(SHIELD_SLOT == "D1"){
        AddDistSensor(EDU_ECHO_PIN_1,EDU_TRIG_PIN_1,"D1");  
      }else if(SHIELD_SLOT == "D2"){
        AddDistSensor(EDU_ECHO_PIN_2,EDU_TRIG_PIN_2,"D2");
      }
      #else
       if(SHIELD_SLOT == "D1"){
        AddDistSensor(SKRIBRAIN_ECHO_PIN_1,SKRIBRAIN_TRIG_PIN_1,"D1");  
      }else if(SHIELD_SLOT == "D2"){
        AddDistSensor(SKRIBRAIN_ECHO_PIN_2,SKRIBRAIN_TRIG_PIN_2,"D2");
      }
      #endif
    }
    void Skribot::AddLineSensor(String SHIELD_SLOT){
      #ifndef ESP_H 
       if(SHIELD_SLOT == "L1"){
        AddLineSensor(EDU_LINE_SENSOR_1,"L1");
      }else if(SHIELD_SLOT == "L2"){
        AddLineSensor(EDU_LINE_SENSOR_2,"L2");
      }else if(SHIELD_SLOT == "L3" ){
        AddLineSensor(EDU_LINE_SENSOR_3,"L3");
      }
      #else
      if(SHIELD_SLOT == "L1"){
        AddLineSensor(SKRIBRAIN_ANALOG_PIN_1,"L1");
      }else if(SHIELD_SLOT == "L2"){
        AddLineSensor(SKRIBRAIN_ANALOG_PIN_2,"L2");
      }else if(SHIELD_SLOT == "L3" ){
        AddLineSensor(SKRIBRAIN_ANALOG_PIN_3,"L3");
      }
      #endif
    }
    void Skribot::AddDCRotor(String SHIELD_SLOT){
      #ifndef ESP_H 
      Serial.println("Adding rotor!");
      if(SHIELD_SLOT == "LEFT"){
        AddDCRotor(EDU_ROTOR_SPEED_PIN_L,EDU_ROTOR_DIR_PIN_L,"Left");          
      }else if(SHIELD_SLOT == "RIGHT"){
        AddDCRotor(EDU_ROTOR_SPEED_PIN_R,EDU_ROTOR_DIR_PIN_R,"Right");
      }
      #else
       Serial.println("Adding rotor!");
       if(SHIELD_SLOT == "LEFT"){
        AddDCRotor(SKRIBRAIN_MOTOR_L_DIR2_PIN,SKRIBRAIN_MOTOR_L_DIR1_PIN,"Left");          
      }else if(SHIELD_SLOT == "RIGHT"){
        AddDCRotor(SKRIBRAIN_MOTOR_R_DIR2_PIN,SKRIBRAIN_MOTOR_R_DIR1_PIN,"Right");
      }
      #endif
    }
   void Skribot::AddLED(String SHIELD_SLOT){
    #ifndef ESP_H 
      if(SHIELD_SLOT == "LED1"){
        AddLED(EDU_LED_DATA_PIN_1,"LED1");
      }else if(SHIELD_SLOT == "LED2"){
        AddLED(EDU_LED_DATA_PIN,"LED2");
      }
      #else
      if(SHIELD_SLOT == "LED1"){
        AddLED(SKRIBRAIN_LED_PIN_1,"LED1");
      }else if(SHIELD_SLOT == "LED2"){
        AddLED(SKRIBRAIN_LED_PIN_2,"LED2");
      }
      #endif
    }

     void Skribot::AddClaw(){
      #ifndef ESP_H 
        AddClaw(EDU_CLAW_PIN1,EDU_CLAW_PIN2);
      #else
      Check_Board_Version();
       switch (Board_type){
          case 1:
            AddClaw(SKRIBRAIN_SERVO_PIN_1,SKRIBRAIN_SERVO_PIN_2);
          break;
          case 2:
            AddClaw(SKRIBRAIN_SERVO_PIN_1,SKRIBRAIN_SERVO_PIN_3);
          break;
          case 3:
            AddClaw(SKRIBRAIN_SERVO_PIN_1,SKRIBRAIN_SERVO_PIN_3);
          break;
          default:
          break;
          }
      #endif
    } 

    void Skribot::AddDistSensor(byte SHIELD_SLOT){
          #ifdef ESP_H 
    if(SHIELD_SLOT == D1_PORT){
        AddDistSensor(SKRIBRAIN_ECHO_PIN_1,SKRIBRAIN_TRIG_PIN_1,D1_PORT);  
    }else if(SHIELD_SLOT == D2_PORT){
        AddDistSensor(SKRIBRAIN_ECHO_PIN_2,SKRIBRAIN_TRIG_PIN_2,D2_PORT);
    }
          #endif
    }

  bool Skribot::AddAbstractDevice(byte pin,byte device_type, byte device_id){
          Abstract_Device *tmp = new Abstract_Device(pin,device_type,device_id);
          Devices[NABSDevices] = tmp;
          NABSDevices++;
          return(Devices[NABSDevices-1]->channel_Init());
    }

   
   bool Skribot::AddSPIDevice(uint8_t MOSI_PIN,uint8_t MISO,uint8_t CLK_PIN,uint8_t CS_PIN,byte id){
    bool tmp = SPIHandler::used_spi_bus<2;
    if(NSPIDevices < 2){
      SPIcomm[id] = new SPIHandler(MOSI_PIN,MISO,CLK_PIN,CS_PIN);
      NSPIDevices++;
    }
    return(tmp);
   }

   bool Skribot::AddI2CDevice(byte _SDA_PIN,byte _CLK_PIN,byte addr,byte id,uint32_t freq){
    bool tmp = I2CHandler::used_i2c_bus<2;
    if(NI2CDevices < 2){
      I2Ccomm[id] = new I2CHandler(_SDA_PIN,_CLK_PIN,freq);
      NI2CDevices++;
      tmp = I2Ccomm[id]->I2Cscan(addr);
   }
   return(tmp);
  }

  bool Skribot::Add_Mono_LED_matrix(uint8_t MOSI_PIN,uint8_t MISO_PIN,uint8_t CLK_PIN,uint8_t CS_PIN,uint8_t id){
     bool tmp = SPIHandler::used_spi_bus<2;
    if(NSPIDevices <2){
      SPIcomm[NSPIDevices] = new SPIHandler(MOSI_PIN,MISO_PIN,CLK_PIN,CS_PIN);
      LED_Matrixes[id] = new Mono_LED_Matrix(SPIcomm[NSPIDevices]);
    }
    SPIcomm[NSPIDevices]->set_SPI_Settings(4000000, MSBFIRST, SPI_MODE0);
    SPIcomm[NSPIDevices]->set_SPI_bit_format(16);

    LED_Matrixes[id]->Init();
    LED_Matrixes[id]->SetIntensity(8);
    LED_Matrixes[id]->Update();
    NSPIDevices++;
    return(tmp);
  }
  void Skribot::AddBuzzer(byte BUZZER_PIN, byte id){
    Buzzers[id] = new Buzzer(BUZZER_PIN,id);
    NBuzzers++;
  }

#endif
  #ifndef _VARIANT_BBC_MICROBIT_

  void Skribot::AddClaw(int Claw_Pin, int Arm_Pin, byte id){
  
    Check_Board_Version();
    if(Board_type == 1){
      Arm_Pin =   SKRIBRAIN_SERVO_PIN_2;
      Claw_Pin =  SKRIBRAIN_SERVO_PIN_1;
    }
 
    Claw *claw = new Claw(Claw_Pin,Arm_Pin,id);
    Claws[NClaws] = claw;
    NClaws++;
  }

   void Skribot::AddLED(int pin,String name,byte N_LED){
    RobotLED *led = new RobotLED(pin,name,N_LED);
    LEDs[NLEDs] = led;
    NLEDs++;
  }

   void Skribot::AddLED(int pin,int id,byte N_LED){
    RobotLED *led = new RobotLED(pin,id,N_LED);
    LEDs[NLEDs] = led;
    NLEDs++;
  }

  #endif



void Skribot::Configure_Connections(String predef){
  Check_Board_Version();
  predefined_hrdw_config = predef;
    if(predef == "MOTHERBOARD"){
      BLE_Set_Module(ESP32_BLE);
      AddBuzzer(37,0);
      pinMode(MOTHERBOARD_BUTTON_PIN,INPUT_PULLUP);
      pinMode(MOTHERBOARD_BUTTON_PIN_2,INPUT_PULLUP);
      // Add_Mono_LED_matrix(MOTHERBOARD_SPI_MOSI,0,MOTHERBOARD_SPI_CLK,MOTHERBOARD_SPI_CS,0);
      // LED_Matrixes[0]->Init();
      // LED_Matrixes[0]->SetRotation(1);
      // LED_Matrixes[0]->SetIntensity(8);
      // LED_Matrixes[0]->ClearDisplay(0);
      // LED_Matrixes[0]->SetChar(0,'X');
      // LED_Matrixes[0]->Update();
      Wire.begin(MOTHERBOARD_I2C_SDA,MOTHERBOARD_I2C_CLK);
      acc_init(Wire);
      mag_init(Wire);
      stausLEDused = false;
    }
    SetSpeed(100);
    Stop();
  }

  bool Skribot::AddGyroscope(byte _SDA_PIN, byte _CLK_PIN,byte id){
      Gyroscope *gyro = new Gyroscope(_SDA_PIN,_CLK_PIN,id);
      gyroscopes[id] = gyro;
      return(gyroscopes[id]->init());
  }

  byte Skribot::getIndexOfDeviceWithPin(byte pin) {
      for (int ii = 0; ii < NABSDevices; ii++) {
          if (Devices[ii]->getPin() == pin)return(ii);
      }
      return(NABSDevices);
  }

  byte Skribot::getIndexOfDeviceWithID(byte id) {
          for (int ii = 0; ii < NABSDevices; ii++) {
          if (Devices[ii]->GetID() == id)return(ii);
      }
      return(NABSDevices);
  }

  byte Skribot::getPinOfDeviceWithID(byte id){
        for (int ii = 0; ii < NABSDevices; ii++) {
          if (Devices[ii]->GetID() == id)return(Devices[ii]->getPin());
      }
      return(NABSDevices);
  }

  bool Skribot::acc_init(TwoWire &Wire){
 
  acc = new LIS3DHTR<TwoWire>();
  acc->begin(Wire,0x19);
  if(!acc)
  {
    Serial.println("Could not start lis3dh");
   return(false);
      ;
  }
  acc->setHighSolution(true);
  acc->setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
  return(true);
}

void Skribot::mag_init(TwoWire &Wire){
  mag = new LIS2MDL(&Wire);
  if(mag->begin() == LIS2MDL_OK)
    Serial.println("Magneto OK");
  else
    Serial.println("Magneto ERROR");	
  mag->Enable();
}

int32_t Skribot::mag_read(int8_t axis){
    int32_t magnetometer[3];
    mag->GetAxes(magnetometer);
    return(magnetometer[axis]);
}

float Skribot::acc_read(uint8_t axis){
   
      float x,y,z;
      acc->getAcceleration(&x,&y,&z);
      switch(axis){
        case 0:
          return(x);
        case 1:
          return(y);
        case 2:
          return(z);
      }
    
    return(0);
}
