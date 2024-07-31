#include "computer/screen.h"
#include <iostream>

Screen::Screen(){

	_setupTextMapping();
	
	metro_line = curr_line = 15;

}

void Screen::display(){

	// Message msg = Message(response.msg.load());
	// bool looper_is_changed = response.looper_state.is_changed.load();

	if ( !response.isEmpty() ){

		switch (response.msg.load())
		{
			case LOOPER:
				_change_looper_state_simple();
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
			// 	_session_change();
				break;
			case SHUTDOWN:
				turnOff();
				break;
			case EMPTY:	// !supress_warning
				std::cerr<<"Screen::display() --> Empty message"<<std::endl;
			case NUM_MESSAGES: break; // !supress_warning
		}
        _display_text();
		response.reset();
    }
}

void Screen::perform_operation(Response response){
	this->response = response;
}

void Screen::_change_looper_state_simple(){
	for (int ch=0;ch<3;++ch){
		// for (int button=0; button<3; ++button)
		looper_states[ch][0].store( response.looper_state.record[ch].load() );
		looper_states[ch][1].store( response.looper_state.playbacks[ch].load() );
		mixer_states[ch].store( response.looper_state.num_tracks[ch].load() );
	}
}

void Screen::_jam_save(){
    jam_save_state = "jam is saved..";
}

void Screen::_toggle_state(){
	toggle_text_states[Control(response.holder.load())].store(response.value.load());
}	

// void Screen::_session_change(){
// 	bool leftwize = (bool)response.value.load();
//     const char* cstate = (leftwize) ? "previous" : "next";
// }
	
void Screen::turnOff(){
    piloop_state = " \x1B[31mOFF\033[0m";
}

void Screen::tick_tock(int intonation){
	// // printf("\033[%d;%dH", row, col);
	if (intonation == 0){
		printf("\033[%d;%dH\33[2K\r", metro_line+3, 0);
		curr_line = metro_line;
	}
	else{
		printf("\033[%d;%dH\33[2K\r", curr_line-1, 0);
	}
	printf("\033[%d;%dH", curr_line++, 0);
	std::cout<<"                         | "<<metro_mapping[intonation]<<" |\n";
}

void Screen::_setupTextMapping(){

	toggle_text_mapping[IN1_ARM][0] = "ARM";
	toggle_text_mapping[IN1_ARM][1] = "\x1B[31mARM\033[0m";
	toggle_text_mapping[IN1_MNTR][0]= "MNTR";
	toggle_text_mapping[IN1_MNTR][1]= "\x1B[31mMNTR\033[0m";
	toggle_text_mapping[IN2_ARM][0] = "ARM";
	toggle_text_mapping[IN2_ARM][1] = "\x1B[31mARM\033[0m";
	toggle_text_mapping[IN2_MNTR][0]= "MNTR";
	toggle_text_mapping[IN2_MNTR][1]= "\x1B[31mMNTR\033[0m";
	
	toggle_text_mapping[IN1_EFF1][0] = "EFF1";
	toggle_text_mapping[IN1_EFF1][1] = "\x1B[32mEFF1\033[0m";
	toggle_text_mapping[IN1_EFF2][0] = "EFF2";
	toggle_text_mapping[IN1_EFF2][1] = "\x1B[33mEFF2\033[0m";
	toggle_text_mapping[IN1_EFF3][0] = "EFF3";
	toggle_text_mapping[IN1_EFF3][1] = "\x1B[34mEFF3\033[0m";
	toggle_text_mapping[IN2_EFF1][0] = "EFF1";
	toggle_text_mapping[IN2_EFF1][1] = "\x1B[32mEFF1\033[0m";
	toggle_text_mapping[IN2_EFF2][0] = "EFF2";
	toggle_text_mapping[IN2_EFF2][1] = "\x1B[33mEFF2\033[0m";
	toggle_text_mapping[IN2_EFF3][0] = "EFF3";
	toggle_text_mapping[IN2_EFF3][1] = "\x1B[34mEFF3\033[0m";
	
	metro_mapping[0] = "\x1B[91mTICK\033[0m";
	metro_mapping[1] = "\x1B[96mtick\033[0m";
	metro_mapping[2] = "\x1B[32mtock\033[0m";

	looper_text_mapping[0][0]="REC";
	looper_text_mapping[0][1]="\x1B[31mREC\033[0m";
	looper_text_mapping[1][0]="PLAY";
	looper_text_mapping[1][1]="\x1B[32mPLAY\033[0m";
	
}

void Screen::initialize_states(DisplayInit comp_states){

	toggle_text_states[IN1_ARM].store(comp_states[0]);
    toggle_text_states[IN1_MNTR].store(comp_states[1]);
    toggle_text_states[IN1_EFF1].store(comp_states[2]);
    toggle_text_states[IN1_EFF2].store(comp_states[3]);
	toggle_text_states[IN1_EFF3].store(comp_states[4]);
#if NUM_INPUTS == 2
    toggle_text_states[IN2_ARM].store(comp_states[5]);
    toggle_text_states[IN2_MNTR].store(comp_states[6]);
    toggle_text_states[IN2_EFF1].store(comp_states[7]);
    toggle_text_states[IN2_EFF2].store(comp_states[8]);
	toggle_text_states[IN2_EFF3].store(comp_states[9]);
#else
	toggle_text_states[IN2_ARM].store(0);
    toggle_text_states[IN2_MNTR].store(0);
    toggle_text_states[IN2_EFF1].store(0);
    toggle_text_states[IN2_EFF2].store(0);
	toggle_text_states[IN2_EFF3].store(0);
#endif
	num_session.store(comp_states[F_NUM_INPUTS*(2+NUM_EFFECTS)]);
	
	for (int ch=0; ch<3; ch++){
		for (int button=0 ; button<2;++button){
			looper_states[ch][button].store(0);
		}
		mixer_states[ch].store(response.looper_state.num_tracks[ch].load());
	}

	jam_save_state = "     -";
	piloop_state = " \x1B[32mON\033[0m ";
	_display_text();
}

void Screen::_display_text(){

	std::cout<< u8"\033[2J\033[1;1H"; 

	std::stringstream display_text;

	display_text<<"-------------- MONITOR -------------  ------ MIXER ------\n" \
    <<"|      Input1          Input2      |  | CH1   CH2   CH3 |\n"
    <<"| "<<toggle_text_mapping[IN1_EFF1][toggle_text_states[IN1_EFF1].load()]<<" "<<toggle_text_mapping[IN1_EFF2][toggle_text_states[IN1_EFF2].load()]<<" "<<toggle_text_mapping[IN1_EFF3][toggle_text_states[IN1_EFF3].load()]<<"    "<<toggle_text_mapping[IN2_EFF1][toggle_text_states[IN2_EFF1].load()]<<" "<<toggle_text_mapping[IN2_EFF2][toggle_text_states[IN2_EFF2].load()]<<" "<<toggle_text_mapping[IN2_EFF3][toggle_text_states[IN2_EFF3].load()]<<" |  |     #tracks     |\n"
    <<"|    "<<toggle_text_mapping[IN1_ARM][toggle_text_states[IN1_ARM].load()]<<"   "<<toggle_text_mapping[IN1_MNTR][toggle_text_states[IN1_MNTR].load()]<<"       "<<toggle_text_mapping[IN2_ARM][toggle_text_states[IN2_ARM].load()]<<"   "<<toggle_text_mapping[IN2_MNTR][toggle_text_states[IN2_MNTR].load()]<<"   |  |  "<<mixer_states[0].load()<<"     "<<mixer_states[1].load()<<"     "<<mixer_states[2].load()<<"  |\n"
    <<"------------------------------------  -------------------\n\n"
    <<" Current              PILOOP-CONSOLE           SAVE_JAM\n"
    <<" "<<"Session:"<<num_session.load()<<"                 "<<piloop_state<<"              "<<jam_save_state<<" \n\n"
    <<"------------------------- LOOPER -------------------------\n"
    <<"|  Start       Channel 1       Channel 2       Channel 3 |\n"
    <<"| "<<"Stop ALL"<<"     "<<looper_text_mapping[0][looper_states[0][0].load()]<<"  "<<looper_text_mapping[1][looper_states[0][1].load()]<<"       "<<looper_text_mapping[0][looper_states[1][0].load()]<<"  "<<looper_text_mapping[1][looper_states[1][1].load()]<<"       "<<looper_text_mapping[0][looper_states[2][0].load()]<<"  "<<looper_text_mapping[1][looper_states[2][1].load()]<<" |\n"
    <<"----------------------------------------------------------\n"
	<<"------------------------ Metronome -----------------------\n"<<std::endl;

	std::cout << display_text.str()<<std::endl;

	response.reset();
}