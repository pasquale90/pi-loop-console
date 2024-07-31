#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>

#include <unistd.h>
#include <wiringPi.h>

// #define GPIO_LED_IN1_EFF1 26
// #define GPIO_LED_IN1_EFF2 19
// #define GPIO_LED_IN2_EFF1 13
// #define GPIO_LED_IN2_EFF2 6
// #define GPIO_LED_IN1_ARM 21
// #define GPIO_LED_IN1_MNTR 20
// #define GPIO_LED_IN2_ARM 16
// #define GPIO_LED_IN2_MNTR 12
// #define GPIO_LED_METRO_R 5
// #define GPIO_LED_METRO_G 0
// #define GPIO_LED_CH1_R 1
// #define GPIO_LED_CH1_G 7
// #define GPIO_LED_CH2_R 25
// #define GPIO_LED_CH2_G 24
// #define GPIO_LED_CH3_R 23
// #define GPIO_LED_CH3_G 22
// #define GPIO_LED_SAVE_JAM 17

int DirectLeds[17] = {26,19,13,6,21,20,16,12,5,0,1,7,25,24,23,22,17};

int main (){

	wiringPiSetupGpio();

	for (int i = 0; i < 17; ++i){
		pinMode(DirectLeds[i], OUTPUT);
		digitalWrite(DirectLeds[i],HIGH);
		delay(500);
		digitalWrite(DirectLeds[i],LOW);
	}

	return 0;
}
