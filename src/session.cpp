#include "session.h"

Session::Session(){
    is_running.store(true);
    is_firsTime.store(true);
    re_initialize.store(true);
    
    monitor.set_stream_buffer(std::bind(&Session::_update_buffers,this, _1, _2) );
}

// no need for method, command is called directly in this.migrate()
// void Session::change_audio_client(){
//     reinitialization.store(true);
// }
// no need for method, command is called directly in this.evacuate()
// void Session::stop_running(){
//     is_running.store(false);
// }

void Session::load(){
    while(is_running){
        if (re_initialize.load()){          
            re_initialize.store(false);
            if (is_firsTime.load()){
                is_firsTime.store(false);
            }else{
                monitor.disconnect();
            }
#if F_NUM_INPUTS == 1
            bool mntr_current_state[F_NUM_INPUTS] = {cfg.get_button_state(IN1_MNTR)};
#elif F_NUM_INPUTS == 2
            bool mntr_current_state[F_NUM_INPUTS] = {cfg.get_button_state(IN1_MNTR),cfg.get_button_state(IN2_MNTR)};
#endif
            monitor.connect(const_cast<char*>(&cfg.currSession_name[0]),mntr_current_state);
        }   
    }
}

void Session::_update_buffers(float *input_buffers[F_NUM_INPUTS],float *output_buffers[F_NUM_OUTPUTS]){
    float** looper_buff = looper.update_buffer(input_buffers);
    mixer.update_buffer(input_buffers,output_buffers,looper_buff);
}

void Session::save(){
// Session::save is responsible for 
//    a) saving config
//    b) saving session (tracks in channels , saved jams ..)

// save config
    cfg.save();
    std::cout<<"Menu::save_session"<<std::endl;

// save session
    looper.save();
    // ...src/session.cpp:4:18

}

void Session::migrate(int next_session){
// Session::migrate is responsible for loading all session data

// 

// load config
    std::string current_session = cfg.currSession_name;
    cfg.open(next_session);
    cfg.display();
    std::cout<<"Session::reset : Session changed from \""<<current_session<<"\" to \""<<cfg.currSession_name<<"\""<<std::endl;
// load other session data
    // ...

    // change_audio_client();
    re_initialize.store(true);
}

void Session::evacuate(){
    is_running.store(false);
    
    // mixer.turnOff();
    // looper.turnOff();
    monitor.disconnect();
}

// void Session::reset2defaults(){
// // clean config
//     cfg.reset();

// // clean other session data 
//     // ...
// }

const char* Session::get_name(){
    // return session_name;
    return cfg.currSession_name.c_str();
}

void Session::set_name(const char* name){
    cfg.currSession_name = name;
}

void Session::notify_session(Control trigger, bool isHold){
    switch (trigger)
    {
//@TODO toggle states within object methods. 
//Session only responsible for calling the right function.
        case CH1_RECDUB:
            std::cout<<"call Looper.recdub(1,isHold)"<<std::endl; // rec/dub/erase_previous
            // std::cout<<"mic_buffer[0] = "<<mic_buffer[0]<<std::endl;
            break;
        case CH1_STOP:
            std::cout<<"call Looper.stoperase(1,isHold)"<<std::endl; // stop/erase_all
            break;
        case CH2_RECDUB:
            std::cout<<"call Looper.recdub(2,isHold)"<<std::endl; 
            break;
        case CH2_STOP:
            std::cout<<"call Looper.stoperase(2,isHold)"<<std::endl; 
            break;
        case CH3_RECDUB:
            std::cout<<"call Looper.recdub(3,isHold)"<<std::endl;
            break;
        case CH3_STOP:
            std::cout<<"call Looper.stoperase(3,isHold)"<<std::endl; 
            break;
        case START_ALL:
            std::cout<<"call Mixer.StartStop()"<<std::endl; // startALL/stopALL
            break;
        case SAVE_JAM:
            std::cout<<"call ??? to save jam? Mixer?? --> Mixer.saveJam() "<<std::endl; 
            break;
        case IN1_ARM:
            cfg.toggle_button_state(IN1_ARM); 
            // std::cout<<"call Monitor.toggle_arm(1)"<<std::endl; 
            std::cout<<"Session::_notify_session IN1_ARM-->"<<cfg.get_button_state(IN1_ARM)<<std::endl;
            break;
        case IN1_MNTR:
            cfg.toggle_button_state(IN1_MNTR);
            std::cout<<"Session::_notify_session IN1_MNTR-->"<<cfg.get_button_state(IN1_MNTR)<<std::endl;

// just for testing mute operation
            if (cfg.get_button_state(IN1_MNTR)){
                monitor.unmute_microphone();
            }else monitor.mute_microphone();


            // std::cout<<"call Monitor.toggle_inout(1)"<<std::endl; 
            std::cout<<"Session::_notify_session -->"<<cfg.get_button_state(IN1_MNTR)<<std::endl;
            break;
        case IN2_ARM:
            cfg.toggle_button_state(IN2_ARM);
            // std::cout<<"call Monitor.toggle_arm(2)"<<std::endl; 
            std::cout<<"Session::_notify_session -->"<<cfg.get_button_state(IN2_ARM)<<std::endl;
            break;
        case IN2_MNTR:
            cfg.toggle_button_state(IN2_MNTR);
            if (cfg.get_button_state(IN2_MNTR)){
                monitor.unmute_instrument();
            }else monitor.mute_instrument();

            // std::cout<<"call Monitor.toggle_inout(2)"<<std::endl; 
            std::cout<<"Session::_notify_session -->"<<cfg.get_button_state(IN2_MNTR)<<std::endl;
            break;
        case IN1_EFF1:
            cfg.toggle_button_state(IN1_EFF1);
            std::cout<<"call ???.toggle_effect(1,1)"<<std::endl; 
            // std::cout<<"Session::_notify_session -->"<<cfg.get_button_state(IN1_EFF1)<<std::endl;
            break;
        case IN1_EFF2:
            cfg.toggle_button_state(IN1_EFF2);
            std::cout<<"call ???.toggle_effect(1,2)"<<std::endl; 
            // std::cout<<"Session::_notify_session -->"<<cfg.get_button_state(IN1_EFF2)<<std::endl;
            break;
        case IN1_EFF3:
            cfg.toggle_button_state(IN1_EFF3);
            std::cout<<"call ???.toggle_effect(1,3)"<<std::endl; 
            // std::cout<<"Session::_notify_session -->"<<cfg.get_button_state(IN1_EFF3)<<std::endl;
            break;
        case IN2_EFF1:
            cfg.toggle_button_state(IN2_EFF1);
            std::cout<<"call ???.toggle_effect(2,1)"<<std::endl; 
            // std::cout<<"Session::_notify_session -->"<<cfg.get_button_state(IN2_EFF1)<<std::endl;
            break;
        case IN2_EFF2:
            cfg.toggle_button_state(IN2_EFF2);
            std::cout<<"call ???.toggle_effect(2,2)"<<std::endl; 
            // std::cout<<"Session::_notify_session -->"<<cfg.get_button_state(IN2_EFF2)<<std::endl;
            break;
        case IN2_EFF3:
            cfg.toggle_button_state(IN2_EFF3);
            std::cout<<"call ???.toggle_effect(2,3)"<<std::endl; 
            // std::cout<<"Session::_notify_session -->"<<cfg.get_button_state(IN2_EFF3)<<std::endl;
            break;
        case TAP_TEMPO:
            std::cout<<"call Metronome.tap()"<<std::endl; 
            break;
        case CH1_VOL_LOW:
            std::cout<<"call Mixer.volume_down(1)"<<std::endl; 
            break;
        case CH1_VOL_HIGH:
            std::cout<<"call Mixer.volume_up(1)"<<std::endl; 
            break;
        case CH2_VOL_LOW:
            std::cout<<"call Mixer.volume_down(2)"<<std::endl; 
            break;
        case CH2_VOL_HIGH:
            std::cout<<"call Mixer.volume_up(2)"<<std::endl; 
            break;
        case CH3_VOL_LOW:
            std::cout<<"call Mixer.volume_down(3)"<<std::endl; 
            break;
        case CH3_VOL_HIGH:
            std::cout<<"call Mixer.volume_up(3)"<<std::endl; 
            break;   
    }
}


