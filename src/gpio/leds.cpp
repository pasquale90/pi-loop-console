#include "gpio/leds.h"

#include <iostream>
Leds::Leds(){

	wiringPiSetupGpio();

	// set up leds (all connected directly to the GPIO pins)
	for (size_t i = 0; i < led_store_helper.size(); ++i){
		pinMode(led_store_helper[i], OUTPUT);
		digitalWrite(led_store_helper[i],LOW);
	}
	_setupLedMapping();
}

void Leds::initialize_leds(DisplayInit states){
	//require fix::for leds you should not have 9 but 8. the 9th int is curr_session can t be displayed using the led interface
	digitalWrite(toggle_led_mapping[IN1_ARM],states[0]);
	digitalWrite(toggle_led_mapping[IN1_MNTR],states[1]);
	digitalWrite(toggle_led_mapping[IN1_EFF1],states[2]);
	digitalWrite(toggle_led_mapping[IN1_EFF2],states[3]);
	// digitalWrite(toggle_led_mapping[IN1_EFF3],states[4]);
#if F_NUM_INPUTS == 2
	digitalWrite(toggle_led_mapping[IN2_ARM],states[5]);
	digitalWrite(toggle_led_mapping[IN2_MNTR],states[6]);
	digitalWrite(toggle_led_mapping[IN2_EFF1],states[7]);
	digitalWrite(toggle_led_mapping[IN2_EFF2],states[8]);
	// digitalWrite(toggle_led_mapping[IN2_EFF3],states[9]);
#endif

	for (size_t ch=0; ch<3; ch++)
		mixer_states[ch].store(0);

	intonation.store(-1);
}

void Leds::display(){

 	if ( !response.isEmpty() ){

		switch (response.msg.load())
		{
			case LOOPER:
				_change_looper_state();
				break;
			case JAM_SAVED:
				_jam_save();
				break;
			case ARM:
				_toggle_state();
				break;
			case MNTR:
				_toggle_state();
				break;
			case EFF:
				_toggle_state();
				break;
			case SESSION_CHANGE:
				_session_change();
				break;
			case SHUTDOWN:
				turnOff();
				break;
			case EMPTY:	// !supress_warning
				std::cerr<<"Screen::display() --> Empty message"<<std::endl;
				break;
			case NUM_MESSAGES: break; // !supress_warning
		}
		response.reset();
    }
	tick_tock();
}

void Leds::perform_operation(Response response){
	this->response = response;	
}

void Leds::set_metro_state(int intonation){
	this->intonation.store(intonation);
}

void Leds::tick_tock(){

	if (intonation.load() == -1)
		return;
	if (intonation.load() == 0){						// is first beat of the loop segment
		digitalWrite(GPIO_LED_METRO_R,HIGH);
		digitalWrite(GPIO_LED_METRO_G,HIGH);
		_blink_playback_channels(true);
		delay(MS_BLINK/2);
		digitalWrite(GPIO_LED_METRO_R,LOW);
		digitalWrite(GPIO_LED_METRO_G,LOW);
		_blink_playback_channels(false);
	}
	if (intonation.load() == 1){						// is first beat of the current measure
		digitalWrite(GPIO_LED_METRO_R,HIGH);
		_blink_playback_channels(true);
		delay(MS_BLINK/2);
		digitalWrite(GPIO_LED_METRO_R,LOW);
		_blink_playback_channels(false);
	}
	if (intonation.load() == 2){						// is not first beat
		digitalWrite(GPIO_LED_METRO_G,HIGH);
		_blink_playback_channels(true);
		delay(MS_BLINK/2);
		digitalWrite(GPIO_LED_METRO_G,LOW);
		_blink_playback_channels(false);
	}
	intonation.store(-1);

}

void Leds::_reset_looper_channels(){

	int blink_times = 4;

	while(blink_times > 0){

		for (int ch =0; ch< 3;++ch){
			if (mixer_states[ch].load()){
				digitalWrite(looper_led_mapping[ch].first, HIGH);
				digitalWrite(looper_led_mapping[ch].second, HIGH);
			}
		}
		delay(MS_BLINK);
		for (int ch =0; ch< 3;++ch){
			digitalWrite(looper_led_mapping[ch].first, LOW);
			digitalWrite(looper_led_mapping[ch].second, LOW);
		}
		
		--blink_times;
	}
}

void Leds::_change_looper_state(){

	bool all_erased = true;
	bool emptyChan = true;
	for (size_t ch=0;ch<3;++ch){
		
		// std::cout<<" Ch "<<ch<<": ";
		// std::cout<< response.looper_state.record[ch].load() << " ";
		// std::cout<<response.looper_state.playbacks[ch].load()<< " ";
		// std::cout<<response.looper_state.num_tracks[ch].load()<<std::endl;

		// REC
		if (response.looper_state.record[ch].load()){
			digitalWrite(looper_led_mapping[ch].first,HIGH);
			all_erased = emptyChan = false;
		}else digitalWrite(looper_led_mapping[ch].first,LOW);

		// PLAY
		playback_states[ch].store(response.looper_state.playbacks[ch].load());
		
		// PAUSE
		if (!response.looper_state.playbacks[ch].load() && response.looper_state.num_tracks[ch].load())
			digitalWrite(looper_led_mapping[ch].second,HIGH);

		
		// UNDUB / ERASE
		int prev_num_tracks = mixer_states[ch].exchange(response.looper_state.num_tracks[ch].load());
		if (mixer_states[ch].load())
		{
			all_erased = false;
			
			// UNDUB
			if (prev_num_tracks > mixer_states[ch].load())
			{
				_undub(ch);		
			}
		}
		else{
			// ERASE
			if (prev_num_tracks){
				_erase(ch);
			}
		}		
	}

	// check if all erased
	if (all_erased)
		_reset_looper_channels();
}

void Leds::_undub(int channel){
	int blink_times = 3;

	while(blink_times > 0){
		digitalWrite(looper_led_mapping[channel].first,HIGH);
		delay(MS_BLINK);
		digitalWrite(looper_led_mapping[channel].first,LOW);
		delay(MS_BLINK);
		--blink_times;
	}
}

void Leds::_erase(int channel){
	int blink_times = 3;

	while(blink_times > 0){
		digitalWrite(looper_led_mapping[channel].first,HIGH);
		digitalWrite(looper_led_mapping[channel].second,HIGH);
		delay(MS_BLINK);
		digitalWrite(looper_led_mapping[channel].first,LOW);
		digitalWrite(looper_led_mapping[channel].second,LOW);
		delay(MS_BLINK);
		--blink_times;
	}
}

void Leds::_jam_save(){
	if(response.value.load() == 0)
	{
		digitalWrite(GPIO_LED_SAVE_JAM,HIGH);
	}
	else
	{
		int blink_times = 8;

		while(blink_times > 0){
			digitalWrite(GPIO_LED_SAVE_JAM,HIGH);
			delay(MS_BLINK);
			digitalWrite(GPIO_LED_SAVE_JAM,LOW);
			delay(MS_BLINK);
			--blink_times;
		}
	}
}

void Leds::_toggle_state(){
	digitalWrite(toggle_led_mapping[Control(response.holder.load())],response.value.load());
}

void Leds::_session_change(){
	
	bool leftwise = (bool)response.value.load();

	if (leftwise){
		
		for (auto i = looper_led_mapping.size(); i >0 ; --i){
			digitalWrite(looper_led_mapping[i-1].second,HIGH);
			delay(MS_BLINK);
			digitalWrite(looper_led_mapping[i-1].first,HIGH);
			delay(MS_BLINK);
		}
		for (auto i = looper_led_mapping.size(); i >0 ; --i){
			digitalWrite(looper_led_mapping[i-1].second,LOW);
			delay(MS_BLINK);
			digitalWrite(looper_led_mapping[i-1].first,LOW);
			delay(MS_BLINK);
		}
	
	}else{
		
		for (size_t i = 0; i <looper_led_mapping.size() ; ++i){
			digitalWrite(looper_led_mapping[i].first,HIGH);
			delay(MS_BLINK);
			digitalWrite(looper_led_mapping[i].second,HIGH);
			delay(MS_BLINK);
		}
		for (size_t i = 0; i <looper_led_mapping.size() ; ++i){
			digitalWrite(looper_led_mapping[i].first,LOW);
			delay(MS_BLINK);
			digitalWrite(looper_led_mapping[i].second,LOW);
			delay(MS_BLINK);
		}

	}

	_reset_looper_channels();
}

void Leds::turnOff(){
	
	for (size_t i = 0; i < led_store_helper.size(); ++i){
		digitalWrite(led_store_helper[i],HIGH);
		delay(MS_BLINK);
	}

	delay(MS_BLINK);
	_jam_save();

	for (size_t i = led_store_helper.size(); i > 0 ; --i){
		delay(MS_BLINK);
		digitalWrite(led_store_helper[i-1],LOW);
	}
}

void Leds::_blink_playback_channels(bool isOn){
	for (size_t ch=0; ch<playback_states.size(); ++ch){
		if ( playback_states[ch].load() ){
			digitalWrite(looper_led_mapping[ch].second,isOn);
		}
	}
}

void Leds::_setupLedMapping(){
	toggle_led_mapping[IN1_ARM] = GPIO_LED_IN1_ARM;
	toggle_led_mapping[IN1_MNTR] = GPIO_LED_IN1_MNTR;
	toggle_led_mapping[IN2_ARM] = GPIO_LED_IN2_ARM;
	toggle_led_mapping[IN2_MNTR] = GPIO_LED_IN2_MNTR;
	toggle_led_mapping[IN1_EFF1] = GPIO_LED_IN1_EFF1;
	toggle_led_mapping[IN1_EFF2] = GPIO_LED_IN1_EFF2;
	toggle_led_mapping[IN2_EFF1] = GPIO_LED_IN2_EFF1;
	toggle_led_mapping[IN2_EFF2] = GPIO_LED_IN2_EFF2;
	
	looper_led_mapping[0] = std::make_pair(GPIO_LED_CH1_R, GPIO_LED_CH1_G);
	looper_led_mapping[1] = std::make_pair(GPIO_LED_CH2_R, GPIO_LED_CH2_G);
	looper_led_mapping[2] = std::make_pair(GPIO_LED_CH3_R, GPIO_LED_CH3_G);
}