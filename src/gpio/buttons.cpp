#include "gpio/buttons.h"
#define VERBOSE_BUTTONS

Buttons::Buttons(){
} 

void Buttons::setup(int base){
	this->base = base;
	// initialize buttons. 16 on the mcp23017, and 4 direct connections
	// wiringPiSetupGpio();
	mcp23017Setup(base, I2CBUS);
	
	_setupControlMapping();
	_initialize_timings();
}

void Buttons::_initialize_timings(){
	
	// for the 16 buttons connected to the mcp23017
	for (int i = 0; i < MCP23017_NUM_BUTTONS; ++i)
		time_pressed[control_mapping[base+i]] = .0;
	
	// for the rest of the buttons which are directly connected to the GPIO pins of the raspberry pi.
	for (int i = 0; i < direct_buttons_helper.size(); ++i){
		time_pressed[control_mapping[direct_buttons_helper.at(i)]] = 0.0;
	}
	/*
	time_pressed[Control(GPIO_ONOFF)] = 0.0;
	time_pressed[Control(GPIO_START_STOP_ALL)] = 0.0;
	time_pressed[Control(GPIO_TAP_TEMPO)] = 0.0;
	time_pressed[Control(GPIO_SAVE_JAM)] = 0.0;
	time_pressed[Control(GPIO_PREV_SESSION)] = 0.0;
	time_pressed[Control(GPIO_NEXT_SESSION)] = 0.0;
	time_pressed[Control(GPIO_ONOFF)] = 0.0;
	*/
}


void Buttons::_setupControlMapping()
{
	//set up the buttons which are connected to the mcp I/O expander
	for (int i = 0; i < MCP23017_NUM_BUTTONS; ++i){
		pinMode(base + i, INPUT);
		pullUpDnControl (base + i, PUD_UP) ;
	}
	//set up the rest of the buttons which are directly connected to the GPIO pins.
	for (int i = 0; i < direct_buttons_helper.size(); ++i){
		pinMode(direct_buttons_helper.at(i), INPUT);
		pullUpDnControl (direct_buttons_helper.at(i), PUD_UP) ;
	}

	control_mapping[base + 10] = CH1_RECDUB;
	control_mapping[base + 11] = CH1_STOP;
	control_mapping[base + 12] = CH2_RECDUB;
	control_mapping[base + 13] = CH2_STOP;
	control_mapping[base + 14] = CH3_RECDUB;
	control_mapping[base + 15] = CH3_STOP;
	control_mapping[GPIO_SAVE_JAM] = SAVE_JAM;
	control_mapping[base + 6] = IN2_ARM;
	control_mapping[base + 7] = IN2_MNTR;
	control_mapping[base + 8] = TAP_TEMPO;
	control_mapping[base + 9] = START_ALL;
	control_mapping[base + 0] = IN1_EFF1;
	control_mapping[base + 1] = IN1_EFF2;
	control_mapping[base + 2] = IN2_EFF1;
	control_mapping[base + 3] = IN2_EFF2;
	control_mapping[base + 4] = IN1_ARM;
	control_mapping[base + 5] = IN1_MNTR;
	control_mapping[GPIO_PREV_SESSION] = PREV_SESSION;
	control_mapping[GPIO_NEXT_SESSION] = NEXT_SESSION;
	// control_mapping[base + 0] = SAVE_SESSION; // SAVE_SESSION
	// control_mapping[base + 0] = CH1_VOL_LOW; //CH1_VOL_LOW
	// control_mapping[base + 0] = CH1_VOL_HIGH; //CH1_VOL_HIGH
	// control_mapping[base + 0] = CH2_VOL_LOW; //CH2_VOL_LOW
	// control_mapping[base + 0] = CH2_VOL_HIGH; //CH2_VOL_HIGH
	// control_mapping[base + 0] = CH3_VOL_LOW; //CH3_VOL_LOW
	// control_mapping[base + 0] = CH3_VOL_HIGH; //CH2_VOL_HIGH
	control_mapping[GPIO_ONOFF] = SHUTDOWN_PILOOP;
}

void Buttons::_helper(int pin, int &released_sig, bool &isHold){
		Control control = control_mapping[pin];

		if (!digitalRead(pin)){										// if pressed
			if(time_pressed[control] == 0.)										// if first time pressed, add the minimum duration in ms
				time_pressed[control] = std::numeric_limits<float>::min();			// add the time elapsed as a float	
			else																// else, add the duration
				time_pressed[control] +=time_elapsed;
		}
		else if (time_pressed[control] != 0.){								// if not pressed, but it was pressed previously, then it was released	
				released_sig = pin;
				if (time_pressed[control] > 2000 )
					isHold = true;
				time_pressed[control] = 0.0; 									// if released, zero-down the time		
		}
}
void Buttons::is_pressed(Trigger &trigger){
	// std::atomic<int> &msg, std::atomic<int> &tval

	present = std::chrono::steady_clock::now();
	time_elapsed = std::chrono::duration_cast< std::chrono::milliseconds >(present - past).count(); // in ms
	past = present;

	int released_sig = -1;
	bool isHold = false;
	int pin;
	// for the mcp buttons
	for (int i = 0; i < MCP23017_NUM_BUTTONS; ++i){
		pin = base + i;
		_helper(pin, released_sig, isHold);
	}
	// for the rest of the buttons
	for (int i = 0; i < direct_buttons_helper.size(); ++i){
		pin = direct_buttons_helper.at(i);
		_helper(pin, released_sig, isHold);
	}

	if (released_sig !=-1){
		// msg.store(control_mapping[released_sig]);
		trigger.control.store(control_mapping[released_sig]);
		if (isHold) 
			trigger.subval.store(1);
		// else tval.store(0);
	}
	// }else{
	// 	msg.store(-1); 
	// }
	// delay(15); //15 ms delay
}
