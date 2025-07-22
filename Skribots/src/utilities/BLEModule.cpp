#include <Arduino.h>
#ifndef _VARIANT_BBC_MICROBIT_
#include "BLEModule.h"

//#define EEPROM_NAME_RESET

#ifdef ESP_H
#define EEPROM_NAME_BEG  40
#define EEPROM_NAME_END  58
#define EEPROM_FLAG_ADDR 59
#define EEPROM_RESET_ADDR 60

bool BLEModule::deviceConnected = false;
bool BLEModule::recivingData = false;
bool BLEModule::OTAupdate = false;
esp_ota_handle_t BLEModule::otaHandler = 0;
char BLEModule::RXBLE_buffer[BLERXBUFFER_SIZE];
int BLEModule::RXBLE_buffer_iterator_end = 1;
int BLEModule::RXBLE_buffer_iterator_beg = 0;
BLECharacteristic* BLEModule::TxCharacteristic;
BLECharacteristic* BLEModule::RxCharacteristic;
long BLEModule::nSends = 0;
long BLEModule::last_OTA_data_recive = 0;
#endif 

void BLEModule::OTA_start(){
	OTAupdate = true;
	esp_ota_begin(esp_ota_get_next_update_partition(NULL), OTA_SIZE_UNKNOWN, &otaHandler);
	delay(3000);
	last_OTA_data_recive = millis();
}
   
void BLEModule::OTA_end(){
    OTAupdate = false;
    nSends = 0;
    if (ESP_OK == esp_ota_set_boot_partition(esp_ota_get_next_update_partition(NULL))) {
    delay(2000);
    esp_restart();
    }
}
bool BLEModule::OTA_write(String ota_data){
	last_OTA_data_recive = millis();
    return(ESP_OK == esp_ota_write(otaHandler, ota_data.c_str(), ota_data.length()));
}

void BLEModule::notify_TXcharacteristic(){
	   char tmp[10];
	   sprintf(tmp,"ack_%d\n",nSends);
	   TxCharacteristic->setValue(tmp);
       TxCharacteristic->notify();
}
void BLEModule::resendRFBF() {
	TxCharacteristic->setValue("READY FOR BINARY FILE\n");
	TxCharacteristic->notify();
}


BLEModule::BLEModule(moduleType type){
	_type = type;
}

char BLEModule::BLE_read(){
	  	char tmp;

switch(_type){
		case HM_10:
		#ifndef ESP_H 
			tmp = Serial3.read();
			#endif
		break;
		case RN4020:
			#ifndef ESP_H 
			tmp = RN4020_read();
			#endif
		break;
	case ESP32_BLE:
		#ifdef ESP_H
		if(BLERXBUFFER_SIZE - substractBufforIterators() > 0){
			incrementRXbuffIterator_beg();
	        tmp = RXBLE_buffer[RXBLE_buffer_iterator_beg];
		}else{
		tmp = 'Q';
		}
		#endif
		break;
		default:
		break;
	}
	    return(tmp);
  }
  


void BLEModule::BLE_write(String msg){
	Serial.print("Sending:");
	Serial.println(msg);
	while(recivingData){
		Serial.println("Waiting for data transfer to end.");
	};
	Serial.println("Sent");
	#ifdef ESP_H
		String _tmp = msg;
	#endif
	switch(_type){
		case HM_10:
			#ifndef ESP_H 
    		Serial3.println(msg);
    		#endif
    		break;
    	case RN4020:
    		#ifndef ESP_H 
    		RN4020_write(msg);
    		#endif
    		break;
    	case ESP32_BLE:
    	#ifdef ESP_H     	 
    		 TxCharacteristic->setValue(_tmp);
        	 TxCharacteristic->notify();
			#endif
    		break;
		default:
			break;
	}
  }

bool BLEModule::BLE_checkConnection(){
			 bool connection;  
	switch(_type){
		case HM_10:
			 connection = digitalRead(EDU_BT_STATE_PIN) == HIGH;
			 break;
		case RN4020:
			#ifndef ESP_H 
			connection = RN4020_checkConnection();
			#endif
			break;
		case ESP32_BLE:
		#ifdef ESP_H
			connection = BLEModule::deviceConnected;
		#endif
			break;
		default:
			break;
	}
			 return(connection);
  }

 int BLEModule::BLE_dataAvailable(){
    		while(recivingData){
    			//Serial.println("Waiting for data transfer to end.");
    			delay(10);
    		};
    		int dataAvalible;
    switch(_type){
		case HM_10:
			#ifndef ESP_H 
    		dataAvalible = Serial3.available();
    		#endif
    		 break;
    	case RN4020:
    		#ifndef ESP_H 
    		dataAvalible = RN4020_dataAvailable();
    		#endif
    		break;
    	case ESP32_BLE:

    	#ifdef ESP_H
    		dataAvalible = BLERXBUFFER_SIZE - substractBufforIterators()-1;
    	#endif

    		break;
		default:
			break;
	}
    		return(dataAvalible);

  }

  void BLEModule::BLE_Setup(){
     byte IfNamed = 0;
     #ifdef ESP_H
     String mac;
     char robot_name_tmp[21] = {' '};
     byte nameFlag;
     #endif
    switch(_type){
		case HM_10:
		#ifndef ESP_H 
		    Serial3.begin(9600);
		    Serial3.setTimeout(50);
		    pinMode(EDU_BT_STATE_PIN,INPUT);
		    IfNamed = EEPROM.read(10);                         
		    if(IfNamed != 1){
		       char MAC[23] = {' '};
		       delay(100);
		       Serial3.write("AT");
		       delay(100);
		       Serial3.write("AT+ADDR?");
		       delay(100);
		       int MACCounter = 0;
		       while(BLE_dataAvailable()){
		        MAC[MACCounter] = Serial3.read();
		        MACCounter++;
		        if(MACCounter > 23)break;
		       }
		        char tmpmess[32] = {' '};
		       sprintf(tmpmess,"AT+NAMESkribot_%c%c%c",MAC[19],MAC[20],MAC[21]);
		       delay(100);
		       Serial3.write(tmpmess);
		       delay(100);
		       Serial3.write("AT+RESET");
		       delay(100);
		    }else{
		      #ifdef DEBUG_MODE
		      Serial.println("Robot named!");
		      #endif
		    }
		   #endif
     break;
     case RN4020:
     	#ifndef ESP_H 
     	  RN4020_Setup();
     	  #endif
     	break;
    case ESP32_BLE:
    #ifdef ESP_H
    	  // Create the BLE Device
		  #ifdef EEPROM_NAME_RESET
		if (EEPROM.read(EEPROM_RESET_ADDR) !=1) {
			nameFlag = 0;
			EEPROM.write(EEPROM_RESET_ADDR, 1);
			EEPROM.commit();
	}
		else {
			nameFlag = 0;
			nameFlag = EEPROM.read(EEPROM_FLAG_ADDR);
		}
		  #else
		  nameFlag = 0;
    	  nameFlag = EEPROM.read(EEPROM_FLAG_ADDR);
		  #endif
#ifdef DEBUG_MODE
    	  Serial.print("Name Flag:");
    	  Serial.println(nameFlag);
    	  #endif
    	  delay(10);
    	  if(nameFlag != 1){
  		  BLEDevice::init("Skribot");
  		  mac = BLEDevice::getAddress().toString();
  		  for(byte rr = 0; rr< mac.length(); rr++)Serial.print(mac[rr]);
  		  sprintf(robot_name_tmp,"Skribrain_%c%c%c",mac[mac.length()-1],mac[mac.length()-2],mac[mac.length()-4]);
  		  Serial.println(robot_name_tmp);
  		  for(byte hh = EEPROM_NAME_BEG; hh<EEPROM_NAME_END+1;hh++){
  		  	EEPROM.write(hh,(byte)robot_name_tmp[hh-EEPROM_NAME_BEG]);
  		  	EEPROM.commit();
  		  	delay(10);
  		  }
  		  EEPROM.write(EEPROM_FLAG_ADDR,1);
  		  EEPROM.commit();
  		  delay(10);
  		  }else{
  		  for(byte jj = EEPROM_NAME_BEG; jj<EEPROM_NAME_END+1;jj++){
  		  robot_name_tmp[jj-EEPROM_NAME_BEG] = EEPROM.read(jj);
  		  delay(10);
  		  }
  		   BLEDevice::init(robot_name_tmp);
  		   //esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT ,ESP_PWR_LVL_N12); //lower tx power for tests

  		  }
		  Serial.println("B");
  		  Server = BLEDevice::createServer();

  		  Server->setCallbacks(new MyServerCallbacks());

    	  Service = Server->createService(SERVICE_UUID);

    	  
    	  RxCharacteristic = Service->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE_NR);

    	  RxCharacteristic->addDescriptor(new BLE2902());

    	  RxCharacteristic->setCallbacks(new MyCallbacks());

    	  TxCharacteristic = Service->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);

    	 TxCharacteristic->addDescriptor(new BLE2902());

    	  Service->start();

  			// Start advertising
    	  Server->getAdvertising()->addServiceUUID(BLEUUID(SERVICE_UUID));
  		  Server->getAdvertising()->start();
		  Serial.println("A");

    #endif
    	break;
	default:
	 break;
	}

  }
#ifdef ESP_H
  void BLEModule::BLE_reset(){
  	delay(500);
	Serial.println("RESET!");
  	Server->startAdvertising();
  }
 #endif

  void BLEModule::BLE_changeName(char *name, bool userConncection){
	  #ifdef ESP_H
  	         char robot_name_tmp[21] = {' '};
  	  #endif
	  switch(_type){
			case HM_10:

				 #ifndef ESP_H 
				  while(BLE_checkConnection()){
				    #ifdef DEBUG_MODE
				    Serial.println("waiting...");
				    #endif
				  }
				    #ifdef DEBUG_MODE
				    Serial.println("disconnected!");
				    #endif
				   char tmpmess[32];
				   delay(100);
				   Serial3.write("AT");
				   delay(100);
				   sprintf(tmpmess,"AT+NAME%s",name);
				   Serial3.write(tmpmess);
				   delay(100);
				   Serial3.write("AT+RESET");
				   delay(100);
				   if(userConncection)EEPROM.write(10,1);
				   #endif
				break;
			case RN4020:
				#ifndef ESP_H 
				RN4020_changeName(name);
				#endif
				break;
			case ESP32_BLE:

			#ifdef ESP_H
				sprintf(robot_name_tmp,"%s",name);
				for(byte hh = EEPROM_NAME_BEG;hh<EEPROM_NAME_END+1;hh++){
  		  			EEPROM.write(hh,(byte)robot_name_tmp[hh-EEPROM_NAME_BEG]);
  		  			EEPROM.commit();
  		  			delay(10);
  		  		}
  		  		Server->getAdvertising()->stop();
  		  		delay(2000);
			#endif

				break;
			default:
				break;
		}
}

#ifdef ESP_H

  void incrementRXbuffIterator_end(){
    	if(BLEModule::RXBLE_buffer_iterator_end == BLERXBUFFER_SIZE-1){
    		BLEModule::RXBLE_buffer_iterator_end = 0;
    	}else{
    		BLEModule::RXBLE_buffer_iterator_end++;
    	}
    }

      void incrementRXbuffIterator_beg(){
    	if(BLEModule::RXBLE_buffer_iterator_beg == BLERXBUFFER_SIZE-1){
    		BLEModule::RXBLE_buffer_iterator_beg = 0;
    	}else{
    		BLEModule::RXBLE_buffer_iterator_beg++;
    	}
    }

    int substractBufforIterators(){
    	if(BLEModule::RXBLE_buffer_iterator_end < BLEModule::RXBLE_buffer_iterator_beg){
    		return(BLEModule::RXBLE_buffer_iterator_beg - BLEModule::RXBLE_buffer_iterator_end);
    	}else if(BLEModule::RXBLE_buffer_iterator_beg < BLEModule::RXBLE_buffer_iterator_end){
    		return(BLERXBUFFER_SIZE - BLEModule::RXBLE_buffer_iterator_end + BLEModule::RXBLE_buffer_iterator_beg);
    	}else{
    		return(0);
    	}
    }

 #endif


  #endif