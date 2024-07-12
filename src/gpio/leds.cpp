#include "gpio/leds.h"

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

	_reset_handlers();

	// // _reset_playbacks
	// for (int ch = 0; ch< 3;++ch)
	// 	playbacks[ch].store(false);

}

void Leds::_reset_handlers(){
	msg_handler.store(-1);
	value_handler.store(-1);
	holders_handler[0].store(-1);
	holders_handler[1].store(-1);
	holders_handler[2].store(-1);
}

void Leds::display(){

	Message msg = Message(msg_handler.load());

    if ( msg != -1 ){

		std::cout<<"Leds --> Something is pressed with msg "<<msg<<std::endl;
		int op[3] = {holders_handler[0].load() ,holders_handler[1].load() ,holders_handler[2].load()};
		int value = value_handler.load();
		switch (msg)
		{
			case PLAY:				// blink G - metro synch
				_play(op, value);
				break;
			case PAUSE:				// set on G
				_pause(op); 
				break;
			case ERASE:				// blink R,G
				_erase(op);
				break;
			case REC:				// set on/off R
				_rec(op, value);
				break;
			case UNDUB:				// blink R		
				_undub(op);
			case RESET:				// blink all channels	
				_reset();
				break;
			case JAM_SAVED:
				_jam_save();
				break;
			case ARM:
				_toggle_state(op[0], value);
				break;
			case MNTR:
				_toggle_state(op[0], value);
				break;
			case EFF:
				_toggle_state(op[0],value);
				break;
			case SESSION_CHANGE:
				_session_change(value);
				break;
			case SHUTDOWN:
				turnOff();
				break;
		}

		_reset_handlers();

    }
}

void Leds::perform_operation(Trigger trigger){

	// while(!triggers.empty()){
		// Trigger trigger = triggers.back();
		// Message msg = trigger.msg;

		msg_handler.store(trigger.msg);
		for (int i=0; i< 3 ; ++i)
			holders_handler[i].store(trigger.holders[i]);
		value_handler.store(trigger.value);

		// triggers.pop_back();
	// }
	
}

void Leds::_play(int channels[3], bool state){
	//
}

void Leds::_pause(int channels[3]){
	int i=0;
	while(i < 3){

		if (channels[i] == -1)
			break;
		else{
			digitalWrite(looper_led_mapping[channels[i]].second, HIGH);
		}
		++i;
	}
}

void Leds::_erase(int channels[3]){
	int i=0;
	while(i < 3){

		if (channels[i] == -1)
			break;
		else{
			digitalWrite(looper_led_mapping[channels[i]].second, LOW);
		}
		++i;
	}

}

void Leds::_rec(int channels[3], bool state){
	int i=0;
	while(i < 3){

		if (channels[i] == -1)
			break;
		else{
			digitalWrite(looper_led_mapping[channels[i]].first, (int)state);
		}
		++i;
	}
}

void Leds::_undub(int channels[3]){

}

void Leds::_reset(){
	int blink_times = 4;

	while(blink_times > 0){

		for (int ch =0; ch< 3;++ch){
			digitalWrite(looper_led_mapping[ch].first, HIGH);
			digitalWrite(looper_led_mapping[ch].second, HIGH);
		}
		delay(MS_BLINK);
		for (int ch =0; ch< 3;++ch){
			digitalWrite(looper_led_mapping[ch].first, LOW);
			digitalWrite(looper_led_mapping[ch].second, LOW);
		}
		
		--blink_times;
	}
}

void Leds::_jam_save(){
	int blink_times = 8;

	while(blink_times > 0){
		digitalWrite(GPIO_LED_SAVE_JAM,HIGH);
		delay(MS_BLINK);
		digitalWrite(GPIO_LED_SAVE_JAM,LOW);
		delay(MS_BLINK);
		--blink_times;
	}
}

void Leds::_toggle_state(int holder, bool val){
	digitalWrite(toggle_led_mapping[Control(holder)],val);
}	

void Leds::_session_change(bool leftwize){

	if (leftwize){
		
		for (int i = looper_led_mapping.size()-1; i >=0 ; --i){
			digitalWrite(looper_led_mapping[i].second,HIGH);
			delay(MS_BLINK);
			digitalWrite(looper_led_mapping[i].first,HIGH);
			delay(MS_BLINK);
		}
		for (int i = looper_led_mapping.size()-1; i >=0 ; --i){
			digitalWrite(looper_led_mapping[i].second,LOW);
			delay(MS_BLINK);
			digitalWrite(looper_led_mapping[i].first,LOW);
			delay(MS_BLINK);
		}
	
	}else{
		
		for (int i = 0; i <looper_led_mapping.size() ; ++i){
			digitalWrite(looper_led_mapping[i].first,HIGH);
			delay(MS_BLINK);
			digitalWrite(looper_led_mapping[i].second,HIGH);
			delay(MS_BLINK);
		}
		for (int i = 0; i <looper_led_mapping.size() ; ++i){
			digitalWrite(looper_led_mapping[i].first,LOW);
			delay(MS_BLINK);
			digitalWrite(looper_led_mapping[i].second,LOW);
			delay(MS_BLINK);
		}

	}
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

/*
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
*/

void Leds::tick_tock(int intonation){
	// std::cout<<"Leds::tick_tock updated with intonation value = "<<intonation<<std::endl;

	if (intonation == 0){						// is first beat of the loop segment
		digitalWrite(GPIO_LED_METRO_R,HIGH);
		digitalWrite(GPIO_LED_METRO_G,HIGH);
		_blink_playback_channels(true);
		delay(50);
		digitalWrite(GPIO_LED_METRO_R,LOW);
		digitalWrite(GPIO_LED_METRO_G,LOW);
		_blink_playback_channels(false);
	}
	if (intonation == 1){						// is first beat of the current measure
		digitalWrite(GPIO_LED_METRO_R,HIGH);
		_blink_playback_channels(true);
		delay(50);
		digitalWrite(GPIO_LED_METRO_R,LOW);
		_blink_playback_channels(false);
	}
	if (intonation == 2){						// is not first beat
		digitalWrite(GPIO_LED_METRO_G,HIGH);
		_blink_playback_channels(true);
		delay(50);
		digitalWrite(GPIO_LED_METRO_G,LOW);
		_blink_playback_channels(false);
	}
}

void Leds::_blink_playback_channels(bool isOn){

	// if (isOn){
	// 	for (int ch=0; ch<playbacks.size(); ++ch){
	// 		bool isPlayback = playbacks[ ch ].load();
	// 		if (isPlayback)
	// 			digitalWrite(looper_led_mapping[ch].second,HIGH);
	// 	}
	// }else{
	// 	for (int ch=0; ch<playbacks.size(); ++ch)
	// 		digitalWrite(looper_led_mapping[ch].second,LOW);
	// }
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
	toggle_led_mapping[IN1_ARM] = GPIO_LED_IN1_ARM;
	toggle_led_mapping[IN1_MNTR] = GPIO_LED_IN1_MNTR;
	toggle_led_mapping[IN2_ARM] = GPIO_LED_IN2_ARM;
	toggle_led_mapping[IN2_MNTR] = GPIO_LED_IN2_MNTR;
	toggle_led_mapping[IN1_EFF1] = GPIO_LED_IN1_EFF1;
	toggle_led_mapping[IN1_EFF2] = GPIO_LED_IN1_EFF2;
	toggle_led_mapping[IN2_EFF1] = GPIO_LED_IN2_EFF1;
	toggle_led_mapping[IN2_EFF2] = GPIO_LED_IN2_EFF2;
	// TAP_TEMPO;
	// PREV_SESSION;
	// NEXT_SESSION;

	looper_led_mapping[0] = std::make_pair(GPIO_LED_CH1_R, GPIO_LED_CH1_G);
	looper_led_mapping[1] = std::make_pair(GPIO_LED_CH2_R, GPIO_LED_CH2_G);
	looper_led_mapping[2] = std::make_pair(GPIO_LED_CH3_R, GPIO_LED_CH3_G);
}



/*
*       for channel:
*       int operation   bool  setOn                             action                                  state
*               0                                       0                               LOW     R                                       stop rec
*               0                                       1                               HIGH R                                  start rec
*               1                                       0                               HIGH G                                  paused
*               1                                       1                               blink G                                 playback
*               2                                       0                               blink G 3 times                 undub
*               2                                       1                               blink GR 3 times                erase
*/

// void Leds::looper_leds(int channel, int operation, bool setOn = false){

//         if(operation == 0){                                                                                     //start/stop recording
//                 auto state = (setOn) ? HIGH : LOW;
//                 digitalWrite(looper_led_mapping[channel].first, state);
//         }
//         else if (operation == 1) {                                                                      //is paused/empty playback channel
//                 if (setOn)
//                         playbacks[channel].store(true);         // blink when playback
//                 else {
//                         playbacks[channel].store(false);
//                         delay(MS_BLINK/2); //just to be sure
//                         digitalWrite(looper_led_mapping[channel].second, HIGH); // HIGH when stopped and not empty
//                 }
//         }

//         else if (operation == 2){                               // undub/erase

//                 int blink_times = 3;

//                 for (int ch=0; ch<blink_times; ch++){

//                         digitalWrite(looper_led_mapping[ch].second,HIGH);
//                         if (setOn)
//                                 digitalWrite(looper_led_mapping[ch].first,HIGH);
//                         delay(MS_BLINK/2);
//                         digitalWrite(looper_led_mapping[ch].second,LOW);
//                         if (setOn)
//                                 digitalWrite(looper_led_mapping[ch].first,LOW);
//                 }
//         }
// }

// void Leds::_initialize_leds(){
// 	std::cout<<"Leds getting led states"<<std::endl;
// 	turnOff();
// 	digitalWrite(toggle_led_mapping[IN1_ARM],1);
// 	std::cout<<"toggle_led_mapping[IN1_ARM] "<<toggle_led_mapping[IN1_ARM];
// 	std::cout<<"(int)cfg.get_button_state(IN1_ARM) "<<(int)cfg.get_button_state(IN1_ARM)<<std::endl;

// 	digitalWrite(toggle_led_mapping[IN1_ARM],(int)cfg.get_button_state(IN1_ARM));
// 	digitalWrite(toggle_led_mapping[IN1_MNTR],(int)cfg.get_button_state(IN1_MNTR));
// 	digitalWrite(toggle_led_mapping[IN2_ARM], (int)cfg.get_button_state(IN2_ARM));
// 	digitalWrite(toggle_led_mapping[IN2_MNTR],(int)cfg.get_button_state(IN2_MNTR));
// 	digitalWrite(toggle_led_mapping[IN1_EFF1],(int)cfg.get_button_state(IN1_EFF1));
// 	digitalWrite(toggle_led_mapping[IN1_EFF2],(int)cfg.get_button_state(IN1_EFF2));
// 	digitalWrite(toggle_led_mapping[IN1_EFF3],(int)cfg.get_button_state(IN1_EFF3));
// 	digitalWrite(toggle_led_mapping[IN2_EFF1],(int)cfg.get_button_state(IN2_EFF1));
// 	digitalWrite(toggle_led_mapping[IN2_EFF2],(int)cfg.get_button_state(IN2_EFF2));
// 	digitalWrite(toggle_led_mapping[IN2_EFF3],(int)cfg.get_button_state(IN2_EFF3));
// 	std::cout<<"Leds led states are set"<<std::endl;
// }
void Leds::initialize_leds(int led_states[8]){

	digitalWrite(toggle_led_mapping[IN1_ARM],led_states[0]);
	digitalWrite(toggle_led_mapping[IN1_MNTR],led_states[1]);
	digitalWrite(toggle_led_mapping[IN2_ARM],led_states[2]);
	digitalWrite(toggle_led_mapping[IN2_MNTR],led_states[3]);
	digitalWrite(toggle_led_mapping[IN1_EFF1],led_states[4]);
	digitalWrite(toggle_led_mapping[IN1_EFF2],led_states[5]);
	// digitalWrite(toggle_led_mapping[IN1_EFF3],button_states[6]);
	digitalWrite(toggle_led_mapping[IN2_EFF1],led_states[6]);
	digitalWrite(toggle_led_mapping[IN2_EFF2],led_states[7]);
	// digitalWrite(toggle_led_mapping[IN2_EFF3],button_states[8]);

}