#include <Arduino.h>
#include "StatusLED.h"
#define STATUS_LED_INTENSIVITY 30
//#define RANDOM_BATTERY_READ
//#define VOLTAGE_DEBUG
#ifndef ESP_H
StatusLED::StatusLED(byte R, byte G, byte B,byte batery){
	 R_pin = R;
	 G_pin = G;
	 B_pin = B;	
	 Battery_pin = batery;
	 pinMode(R_pin,OUTPUT);
	 pinMode(G_pin,OUTPUT);
	 pinMode(B_pin,OUTPUT);
	 Current_color = OFF;
	 pinMode(Battery_pin,ANALOG);
}
void StatusLED::TurnOn(color _color){
if(_color != Current_color){
	switch(_color){
	case RED:
 	 digitalWrite(R_pin,LOW);
	 digitalWrite(G_pin,HIGH);
	 digitalWrite(B_pin,HIGH);
	break;
	case GREEN:
 	 digitalWrite(R_pin,HIGH);
	 digitalWrite(G_pin,LOW);
	 digitalWrite(B_pin,HIGH);
	break;
	case BLUE:
 	 digitalWrite(R_pin,HIGH);
	 digitalWrite(G_pin,HIGH);
	 digitalWrite(B_pin,LOW);
	break;
	case YELLOW:
 	 digitalWrite(R_pin,LOW);
	 digitalWrite(G_pin,LOW);
	 digitalWrite(B_pin,HIGH);
	break;
	case PURPLE:
 	 digitalWrite(R_pin,LOW);
	 digitalWrite(G_pin,HIGH);
	 digitalWrite(B_pin,LOW);
	break;
	case WHITE:
 	 digitalWrite(R_pin,LOW);
	 digitalWrite(G_pin,LOW);
	 digitalWrite(B_pin,LOW);
	break;
	case OFF:
	 digitalWrite(R_pin,HIGH);
	 digitalWrite(G_pin,HIGH);
	 digitalWrite(B_pin,HIGH);
	break;
	default:
	break;
	}
	Current_color = _color;
}
}
#else
StatusLED::StatusLED(byte LED_PIN,byte batery,byte V){
	status = new RobotLED(LED_PIN,0,2);
	Current_color = OFF;
	Battery_pin = batery;
	Board_V = V;
	//if(V > 2)analogSetPinAttenuation(GPIO_NUM_35, ADC_11db);			//ESP core bugfix for ADC channel1 
}
void StatusLED::TurnOn(color _color,byte NLED){
if(_color != Current_color){
	switch(_color){
	case RED:
 		status->turnON(STATUS_LED_INTENSIVITY,0,0,NLED);
	break;
	case GREEN:
 	 	status->turnON(0,STATUS_LED_INTENSIVITY,0,NLED);
	break;
	case BLUE:
 	 	status->turnON(0,0,STATUS_LED_INTENSIVITY,NLED);
	break;
	case YELLOW:
 	 status->turnON(STATUS_LED_INTENSIVITY,STATUS_LED_INTENSIVITY,0,NLED);
	break;
	case PURPLE:
 	status->turnON(STATUS_LED_INTENSIVITY,0,STATUS_LED_INTENSIVITY,NLED);
	break;
	case WHITE:
 	status->turnON(STATUS_LED_INTENSIVITY,STATUS_LED_INTENSIVITY,STATUS_LED_INTENSIVITY,NLED);
	break;
	case OFF:
	 status->turnON(0,0,0,NLED);
	break;
	default:
	break;
	}
	Current_color = _color;
}
}
#endif

float StatusLED::ReadADC(){
  //esp_adc_cal_characteristics_t adc_chars;
  //esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  int raw_read = analogRead(Battery_pin);
  //uint32_t milivolts = esp_adc_cal_raw_to_voltage(raw_read, &adc_chars);
  float Voltage = 0;
  return(Voltage);
}
int StatusLED::CheckBateryStatus(bool force_led_update){
	#ifdef ESP_H
	float Voltage;
	byte yy = 0;
	if(Board_V == 1){
	for(int ii = 0 ; ii <100;ii++){
		if(digitalRead(Battery_pin) == HIGH){
			yy++;
		}	
	}
	if(yy > 95){
		Current_voltage = 12.0;
	}else{
		Current_voltage = 0.0;
	}
	}else{
	Voltage = 0;//ReadADC();
	Current_voltage = Voltage*4.91;
	}
	#ifdef VOLTAGE_DEBUG
	Serial.print("VOLTAGE:");
	Serial.println(Voltage);
	#endif
	#else
	float mult = 0.005*3;
	Current_voltage = (float)analogRead(Battery_pin)*mult;
	#endif
	return(ReadBatteryState(force_led_update));
}
byte StatusLED::ReadBatteryState(bool force_led_update){
	byte read;
	if(Current_voltage > 9.3){
		read = (byte)((Current_voltage-9.3)/2.5*100);
	}else{
		status->turnON(0, STATUS_LED_INTENSIVITY,0, 1);
		return(100);
	}
	#ifdef RANDOM_BATTERY_READ
	read = millis()%100;
	#endif
	if(Board_V == 2){
		if(read > 50){
			TurnOn(GREEN);
		}else if(read > 20){
			TurnOn(YELLOW);
		}else{
			TurnOn(RED);
		}
	}else{
		byte R = 0;
		byte G = 0;
		if(read > 70){
			R = 0;
			G = STATUS_LED_INTENSIVITY;
		}else if(read > 50){
			R = STATUS_LED_INTENSIVITY-(STATUS_LED_INTENSIVITY*(float)(read-50)/20);
			G = STATUS_LED_INTENSIVITY;
		}else if(read > 30){
			R = STATUS_LED_INTENSIVITY;
			G = STATUS_LED_INTENSIVITY*(float)(read-30)/20;
		}else{
			R = STATUS_LED_INTENSIVITY;
			G = 0;
		}
	if(force_led_update || (R != current_R) || (G != current_G)){
	   status->turnON(R,G,0,1);
	   current_G =G;
	   current_R =R;
	}
	}
	return(read);
}

// version f < 3.2 green blink
// version 3.2 2 x blue blink
void StatusLED::BLINK_OK(){
	 for(byte rr = 0;rr<2;rr++){
	 TurnOn(OFF);
	 delay(200);
	 TurnOn(BLUE);
	 delay(200);
	}
	

}