#ifndef LEDS_H
#define LEDS_H

#define MS_BLINK 25 //25 ms blink time

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
#include <utility>
#include <atomic>
#include <wiringPi.h> 
#include "controls.h"
#include "triggers.h"

/*! @brief LEDs as the GPIO-based output interface*/
class Leds{

    public:
		Leds();       
		void initialize_leds(int[8]);

		void display();
		void perform_operation(Trigger);
		void tick_tock(int);
		// void looper_leds(int,int,bool)
		
		void turnOff();

    private: 
        void _setupLedMapping();
		std::unordered_map<Control,int> toggle_led_mapping;
		std::unordered_map<int, std::pair<int, int>> looper_led_mapping;

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

	std::atomic<int> msg_handler,value_handler,holders_handler[3];
	// std::atomic<bool> playbacks[3];
	// helping functions
	void _blink_playback_channels(bool);

	void _reset_handlers();
	void _play(int[3], bool);
	void _pause(int[3]); 
	void _erase(int[3]);
	void _rec(int[3], bool);
	void _undub(int[3]);
	void _reset();
	void _jam_save();
	void _toggle_state(int, bool);
	void _session_change(bool);
		
		// void blink_all(); // for when changing session
		// void set_on(Control);
		// void set_off(Control);
};

#endif
