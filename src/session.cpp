#include "session.h"

#include <ctime>
#include <cstring>
#include <string>

Session::Session(){
    is_running.store(true);
    is_firsTime.store(true);
    re_initialize.store(true);

    
#if F_NUM_INPUTS == 1
    bool effects_curr_state[NUM_INPUTS][NUM_EFFECTS] = {cfg.get_button_state(IN1_EFF1),cfg.get_button_state(IN1_EFF2),cfg.get_button_state(IN1_EFF3)};
#elif F_NUM_INPUTS == 2
    bool effects_curr_state[NUM_INPUTS][NUM_EFFECTS] = {
        {cfg.get_button_state(IN1_EFF1),cfg.get_button_state(IN1_EFF2),cfg.get_button_state(IN1_EFF3)},
        {cfg.get_button_state(IN2_EFF1),cfg.get_button_state(IN2_EFF2),cfg.get_button_state(IN2_EFF3)}};
#endif
    monitor.initialize_effects(effects_curr_state);
    monitor.set_stream_buffer(std::bind(&Session::_update_buffers,this, std::placeholders::_1, std::placeholders::_2) );

    _initialize_leds();

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
            monitor.connect(const_cast<char*>(&cfg.currSession_name[0]));
        }   
    }
}

void Session::load_metronome(){
    looper.load_metronome();
}

void Session::_update_buffers(float *input_buffers[F_NUM_INPUTS],float *output_buffers[F_NUM_OUTPUTS]){
#if F_NUM_INPUTS == 1
    bool monitorIn[F_NUM_INPUTS]={cfg.get_button_state(IN1_MNTR)};
    bool armEnabled[F_NUM_INPUTS]={cfg.get_button_state(IN1_ARM)};
#elif F_NUM_INPUTS == 2
    bool monitorIn[F_NUM_INPUTS]={cfg.get_button_state(IN1_MNTR),cfg.get_button_state(IN2_MNTR)};
    bool armEnabled[F_NUM_INPUTS]={cfg.get_button_state(IN1_ARM),cfg.get_button_state(IN2_ARM)};
#endif

    // float** looper_buff = looper.update_buffer(input_buffers,armEnabled);
    std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> looper_buff = looper.update_buffer(input_buffers,armEnabled);
    // float *looper_buff[F_NUM_OUTPUTS] = {0};
    mixer.update_buffer(input_buffers,output_buffers,looper_buff,monitorIn);
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

    looper.reset();
// @TODO mixer reset
    // mixer.reset();
    leds.blink_all();
    _initialize_leds();
}

void Session::evacuate(){
    is_running.store(false);
    
    leds.turnOff();
    // mixer.turnOff();
    looper.reset();
    monitor.disconnect();
}

// void Session::reset2defaults(){
// // clean config
//     cfg.reset();

// // clean other session data 
//     // ...
// }

void Session::save_jam(){

    std::string savepath = cfg.jam_savepath + "/" + _get_datetime();
    mixer.save_jam(savepath + ".wav");
// @TODO create metadata for the file. This will server as a method to import this file in DAWs to be used latter as a songwritting guide.
    // save tempo. save rythm
}

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
            looper.recdub(0,isHold);
            break;
        case CH1_STOP:
        // @TODO --> tap tempo?
            looper.stoperase(0,isHold); 
            break;
        case CH2_RECDUB:
            looper.recdub(1,isHold);
            break;
        case CH2_STOP:
            looper.stoperase(1,isHold);
            break;
        case CH3_RECDUB:
            looper.recdub(2,isHold);
            break;
        case CH3_STOP:
            looper.stoperase(2,isHold);
            break;
        case START_ALL:
            looper.start_stop_all(isHold); // startALL/stopALL
            break;
        case SAVE_JAM:
            if (isHold)
                save_jam();
            break;
        case IN1_ARM:
            cfg.toggle_button_state(IN1_ARM); 
            
            if (cfg.get_button_state(IN1_ARM)){
                leds.set_on(IN1_ARM);
            }else leds.set_off(IN1_ARM);
            break;
        case IN1_MNTR:
            cfg.toggle_button_state(IN1_MNTR);
            
            if (cfg.get_button_state(IN1_MNTR)){
                leds.set_on(IN1_MNTR);
            }else leds.set_off(IN1_MNTR);
            break;
        case IN2_ARM:
            cfg.toggle_button_state(IN2_ARM);
            if (cfg.get_button_state(IN2_ARM)){
                leds.set_on(IN2_ARM);
            }else leds.set_off(IN2_ARM);
            break;
        case IN2_MNTR:
            cfg.toggle_button_state(IN2_MNTR);
            
            if (cfg.get_button_state(IN2_MNTR)){
                leds.set_on(IN2_MNTR);
            }else leds.set_off(IN2_MNTR);
            break;
        case IN1_EFF1:
            cfg.toggle_button_state(IN1_EFF1);
            monitor.toggle_effect(0,0,cfg.get_button_state(IN1_EFF1));
            if (cfg.get_button_state(IN1_EFF1)){
                leds.set_on(IN1_EFF1);
            }else leds.set_off(IN1_EFF1);
            break;
        case IN1_EFF2:
            cfg.toggle_button_state(IN1_EFF2);
            monitor.toggle_effect(0,1,cfg.get_button_state(IN1_EFF2));
            if (cfg.get_button_state(IN1_EFF2)){
                leds.set_on(IN1_EFF2);
            }else leds.set_off(IN1_EFF2);
            break;
        case IN1_EFF3:
            cfg.toggle_button_state(IN1_EFF3);
            monitor.toggle_effect(1,2,cfg.get_button_state(IN1_EFF3));
            // if (cfg.get_button_state(IN1_EFF3)){
            //     leds.set_on(IN1_EFF3);
            // }else leds.set_off(IN1_EFF3);
            break;
        case IN2_EFF1:
            cfg.toggle_button_state(IN2_EFF1);
            monitor.toggle_effect(1,0,cfg.get_button_state(IN2_EFF1));
            if (cfg.get_button_state(IN2_EFF1)){
                leds.set_on(IN2_EFF1);
            }else leds.set_off(IN2_EFF1);
            break;
        case IN2_EFF2:
            cfg.toggle_button_state(IN2_EFF2);
            monitor.toggle_effect(1,1,cfg.get_button_state(IN2_EFF2));
            if (cfg.get_button_state(IN2_EFF2)){
                leds.set_on(IN2_EFF2);
            }else leds.set_off(IN2_EFF2);
            break;
        case IN2_EFF3:
            cfg.toggle_button_state(IN2_EFF3);
            monitor.toggle_effect(1,2,cfg.get_button_state(IN2_EFF3));
            // if (cfg.get_button_state(IN2_EFF3)){
            //     leds.set_on(IN2_EFF3);
            // }else leds.set_off(IN2_EFF3);
            break;
        case TAP_TEMPO:
            looper.tap_alter_metronome(isHold);
            break;
        case CH1_VOL_LOW:
            looper.volume_down(0);
            break;
        case CH1_VOL_HIGH:
            looper.volume_up(0);
            break;
        case CH2_VOL_LOW:
            looper.volume_down(1);
            break;
        case CH2_VOL_HIGH:
            looper.volume_up(1);
            break;
        case CH3_VOL_LOW:
            looper.volume_down(2);
            break;
        case CH3_VOL_HIGH:
            looper.volume_up(2);
            break;   
    }
    // cfg.display();
    looper.display_states();
}

std::string Session::_parse_time_val(int value){
    int num_digits = 0;
    int temp = value;
    while (temp){   
    temp /= 10;
    ++num_digits;
    }
    if (num_digits == 1){
        return "0"+std::to_string(value);
    }else if (num_digits == 2){
        return std::to_string(value);
    }else if (num_digits > 2){
        return std::to_string(value).substr (num_digits-2,2);
    }
}

std::string Session::_get_datetime(){ 
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);

    return _parse_time_val(now->tm_year + 1900)
         + _parse_time_val (now->tm_mon + 1)
         +  _parse_time_val(now->tm_mday)  + '_' 
         +  _parse_time_val(now->tm_hour)
         +  _parse_time_val(now->tm_min)
         +  _parse_time_val(now->tm_sec);
}

void Session::_initialize_leds(){
    if (cfg.get_button_state(IN1_ARM)){
        leds.set_on(IN1_ARM);
    }
    if (cfg.get_button_state(IN1_MNTR)){
        leds.set_on(IN1_MNTR);
    }
    if (cfg.get_button_state(IN2_ARM)){
        leds.set_on(IN2_ARM);
    }
    if (cfg.get_button_state(IN2_MNTR)){
        leds.set_on(IN2_MNTR);}

    if (cfg.get_button_state(IN1_EFF1)){
        leds.set_on(IN1_EFF1);
    }
    if (cfg.get_button_state(IN1_EFF2)){
        leds.set_on(IN1_EFF2);
    }
    // if (cfg.get_button_state(IN1_EFF3)){
    //     leds.set_on(IN1_EFF3);
    // }
    if (cfg.get_button_state(IN2_EFF1)){
        leds.set_on(IN2_EFF1);
    }
    if (cfg.get_button_state(IN2_EFF2)){
        leds.set_on(IN2_EFF2);
    }
    // if (cfg.get_button_state(IN2_EFF3)){
    //     leds.set_on(IN2_EFF3);
    // }
}