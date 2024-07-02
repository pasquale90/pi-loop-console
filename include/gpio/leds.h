#ifndef LEDS_H
#define LEDS_H

#define GPIO_LED_IN1_EFF1 26
#define GPIO_LED_IN1_EFF2 19
#define GPIO_LED_IN2_EFF1 13
#define GPIO_LED_IN2_EFF2 6
#define GPIO_LED_IN1_ARM 21
#define GPIO_LED_IN1_MNTR 20
#define GPIO_LED_IN2_ARM 16
#define GPIO_LED_IN2_MNTR 12
#define GPIO_LED_METRO_R 5
#define GPIO_LED_METRO_G 0
#define GPIO_LED_CH1_R 1
#define GPIO_LED_CH1_G 7
#define GPIO_LED_CH2_R 25
#define GPIO_LED_CH2_G 24
#define GPIO_LED_CH3_R 23
#define GPIO_LED_CH3_G 22
#define GPIO_LED_SAVE_JAM 17
// #define GPIO_LED_CH1 ..
// #define GPIO_LED_CH2 ..
// #define GPIO_LED_CH3 ..

#include <array>
#include <unordered_map>
#include <unistd.h>
#include <wiringPi.h> 

#include "controls.h"


class Leds{

    public:
		Leds();       
		void turnOff();
		void blink_all(); // for when changing session
		void set_on(Control);
		void set_off(Control);

		
		// void tick_tock(int);
		// void toggle_effects(int,int,bool);

    private: 
        void _setupLedMapping();
		std::unordered_map<Control,int> led_mapping;

		std::array<int,17> led_store_helper = {
											GPIO_LED_IN1_EFF1, \
											GPIO_LED_IN1_EFF2, \
											GPIO_LED_IN2_EFF1, \
											GPIO_LED_IN2_EFF2, \
											GPIO_LED_IN1_ARM, \
											GPIO_LED_IN1_MNTR, \
											GPIO_LED_IN2_ARM, \
											GPIO_LED_IN2_MNTR, \
											GPIO_LED_METRO_R, \
											GPIO_LED_METRO_G, \
											GPIO_LED_CH1_R, \
											GPIO_LED_CH1_G, \
											GPIO_LED_CH2_R, \
											GPIO_LED_CH2_G, \
											GPIO_LED_CH3_R, \
											GPIO_LED_CH3_G, \
											GPIO_LED_SAVE_JAM \
										};
};

#endif
