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
#include "trigger.h"
#include "response.h"
#include "audio_settings.h"

using DisplayInit = int[(F_NUM_INPUTS*(2+NUM_EFFECTS))+1];

/*! @brief LEDs as the GPIO-based output interface*/
class Leds{

    public:
		Leds();       
		void initialize_leds(DisplayInit);
		void display();
		void perform_operation(Response);
		void set_metro_state(int);
		void tick_tock();		
		void turnOff();

    private: 
        void _setupLedMapping();
		std::unordered_map<Control,int> toggle_led_mapping;
		std::unordered_map<int, std::pair<int, int>> looper_led_mapping;
		std::array<std::atomic<int>,3> mixer_states;
		std::array<std::atomic<int>,3> playback_states;
		Response response;
		std::atomic<int> intonation;
		std::array<int,17> led_store_helper = {
											GPIO_LED_IN1_EFF1, \
											GPIO_LED_IN1_EFF2, \
											GPIO_LED_IN2_EFF1, \
											GPIO_LED_IN2_EFF2, \
											GPIO_LED_METRO_R, \
											GPIO_LED_METRO_G, \
											GPIO_LED_IN1_ARM, \
											GPIO_LED_IN1_MNTR, \
											GPIO_LED_IN2_ARM, \
											GPIO_LED_IN2_MNTR, \
											GPIO_LED_CH1_R, \
											GPIO_LED_CH1_G, \
											GPIO_LED_CH2_R, \
											GPIO_LED_CH2_G, \
											GPIO_LED_CH3_R, \
											GPIO_LED_CH3_G, \
											GPIO_LED_SAVE_JAM \
										};

	// helping functions
	void _blink_playback_channels(bool);
	void _reset_looper_channels();
	void _undub(int);
	void _erase(int);
	void _change_looper_state();
	void _jam_save();
	void _toggle_state();
	void _session_change();
};

#endif
