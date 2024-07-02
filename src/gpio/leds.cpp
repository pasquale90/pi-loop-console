#include "leds.h"

#include <iostream>
Leds::Leds(){
	std::cout<<"\n\n\n\n\t\t\t\tLeds constuctor called \n\n\n\n"<<std::endl;
	wiringPiSetupGpio();

	// set up leds (all connected directly to the GPIO pins)
	for (int i = 0; i < led_store_helper.size(); ++i){
		pinMode(led_store_helper[i], OUTPUT);
		digitalWrite(led_store_helper[i],LOW);
	}
	_setupLedMapping();
}

void Leds::turnOff(){
	for (int i = 0; i < led_store_helper.size(); ++i){
		digitalWrite(led_store_helper[i],HIGH);
		delay(50);
		digitalWrite(led_store_helper[i],LOW);
	}
	for (int i = led_store_helper.size()-2; i > 0; --i){
		digitalWrite(led_store_helper[i],HIGH);
		delay(50);
		digitalWrite(led_store_helper[i],LOW);
	}
}

void Leds::blink_all(){
	for (int i = 0; i < led_store_helper.size(); ++i){
		digitalWrite(led_store_helper[i],HIGH);
	}
	delay(50);
	for (int i = 0; i < led_store_helper.size(); ++i){
		digitalWrite(led_store_helper[i],LOW);
	}
}

void Leds::set_on(Control control){
	digitalWrite(led_mapping[control],HIGH);
}

void Leds::set_off(Control control){
	digitalWrite(led_mapping[control],LOW);
}


void Leds::_setupLedMapping(){
	// CH1_RECDUB
	// CH1_STOP;
	// CH2_RECDUB;
	// CH2_STOP;
	// CH3_RECDUB;
	// CH3_STOP;
	// START_ALL;
	// SAVE_JAM;
	led_mapping[IN1_ARM] = GPIO_LED_IN1_ARM;
	led_mapping[IN1_MNTR] = GPIO_LED_IN1_MNTR;
	led_mapping[IN2_ARM] = GPIO_LED_IN2_ARM;
	led_mapping[IN2_MNTR] = GPIO_LED_IN2_MNTR;
	led_mapping[IN1_EFF1] = GPIO_LED_IN1_EFF1;
	led_mapping[IN1_EFF2] = GPIO_LED_IN1_EFF2;
	led_mapping[IN2_EFF1] = GPIO_LED_IN2_EFF1;
	led_mapping[IN2_EFF2] = GPIO_LED_IN2_EFF2;
	// TAP_TEMPO;
	// PREV_SESSION;
	// NEXT_SESSION;
}
