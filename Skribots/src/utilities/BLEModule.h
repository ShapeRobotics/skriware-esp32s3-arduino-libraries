#ifndef BLE_MOD_H
#define BLE_MOD_H
#include "Arduino.h"
#include "esp_ota_ops.h"
#define EDU_BT_STATE_PIN 3
#ifdef ESP_H
#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"
#include "EEPROM.h"
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define BLERXBUFFER_SIZE 520
#else
#include "RN4020.h"
#endif
	enum moduleType {HM_10 = 0,RN4020 = 1,ESP32_BLE = 2};
  
	class BLEModule
{
public:
	BLEModule(moduleType type);
	char BLE_read();       ;                                                 
    void BLE_write(String msg);
    bool BLE_checkConnection();
    int  BLE_dataAvailable();
    void BLE_changeName(char name[],bool userConncection = true);
    void BLE_Setup();
    void BLE_reset();

    static void OTA_start();
    static void OTA_end();
    static bool OTA_write(String ota_data);                       // functions for OTA update
    static void notify_TXcharacteristic();
    static void resendRFBF();

    #ifdef ESP_H
        static bool deviceConnected,recivingData,OTAupdate;
        static char RXBLE_buffer[BLERXBUFFER_SIZE];
        static int RXBLE_buffer_iterator_end;
        static int RXBLE_buffer_iterator_beg;
        static esp_ota_handle_t otaHandler;
        static long nSends;
        static long last_OTA_data_recive;
    #endif
private:
	moduleType	_type;
    #ifdef ESP_H
    BLEServer *Server = NULL;
    BLEService *Service; 
    static BLECharacteristic *TxCharacteristic;
    static BLECharacteristic *RxCharacteristic;
    #endif

};
void incrementRXbuffIterator_beg();
void incrementRXbuffIterator_end();
int substractBufforIterators();


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      BLEModule::deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      BLEModule::deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      BLEModule::recivingData = true;
      char ascii;
      String rxValue = pCharacteristic->getValue();
      if(!BLEModule::OTAupdate){
      Serial.println("*********************");
      Serial.println("New message package:");
      if (rxValue.length() > 0) {
        for (int i = 0; i < rxValue.length(); i++){
          if(substractBufforIterators() < 1)break;
          ascii = rxValue[i];
          Serial.print(ascii);
          BLEModule::RXBLE_buffer[BLEModule::RXBLE_buffer_iterator_end] = ascii;
          incrementRXbuffIterator_end();
        }
      }
    Serial.println("End of message package.");
    Serial.println("*********************");
    }else{
      if(rxValue != "L\nU\n" && rxValue != "U\n") {
        bool tmp = BLEModule::OTA_write(rxValue);
        if(tmp!=1)Serial.println(tmp);
        if(tmp){
          BLEModule::notify_TXcharacteristic();
          BLEModule::nSends++;
        }
      }else{
          BLEModule::resendRFBF();
      }
    }
      BLEModule::recivingData = false;
      
    }
};


#endif