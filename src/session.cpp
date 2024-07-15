#include "session.h"

#include <ctime>
#include <cstring>
#include <string>
#define MS_SLEEP_ 25
#include <chrono>
#include <thread>

Session::Session(){
    is_running.store(true);
    is_firsTime.store(true);
    re_initialize.store(true);
    monitor.set_stream_buffer(std::bind(&Session::_update_buffers,this, std::placeholders::_1, std::placeholders::_2) );
}

void Session::setup(){
#if F_NUM_INPUTS == 1
    bool effects_curr_state[NUM_INPUTS][NUM_EFFECTS] = {cfg.get_button_state(IN1_EFF1),cfg.get_button_state(IN1_EFF2),cfg.get_button_state(IN1_EFF3)};
#elif F_NUM_INPUTS == 2
    bool effects_curr_state[NUM_INPUTS][NUM_EFFECTS] = {
        {cfg.get_button_state(IN1_EFF1),cfg.get_button_state(IN1_EFF2),cfg.get_button_state(IN1_EFF3)},
        {cfg.get_button_state(IN2_EFF1),cfg.get_button_state(IN2_EFF2),cfg.get_button_state(IN2_EFF3)}};
#endif
    monitor.initialize_effects(effects_curr_state);
    
    monitor.update_states(false, 0, cfg.get_button_state(IN1_ARM));
    monitor.update_states(true, 0, cfg.get_button_state(IN1_MNTR));
#if F_NUM_INPUTS == 2
    monitor.update_states(false, 1, cfg.get_button_state(IN2_ARM));
    monitor.update_states(true, 1, cfg.get_button_state(IN2_MNTR));
#endif

    bool arm_states[F_NUM_INPUTS];
    bool mntr_states[F_NUM_INPUTS];
    monitor.get_states(arm_states,false);
    monitor.get_states(mntr_states,true);

    std::cout<<"Session setup \n"
    <<"effects_curr_state[0][0] "<<effects_curr_state[0][0]<<"\n"
    <<"effects_curr_state[0][1] "<<effects_curr_state[0][1]<<"\n"
    <<"effects_curr_state[1][0] "<<effects_curr_state[1][0]<<"\n"
    <<"effects_curr_state[1][1] "<<effects_curr_state[1][1]<<"\n"
    <<"arm input 1 "<<arm_states[0]<<"\n"
    <<"monitor input 1 "<<mntr_states[0]<<"\n"
    <<"arm input 2 "<<arm_states[1]<<"\n"
    <<"monitor input 2 "<<mntr_states[1]<<"\n";
 
    std::cout<<"Session setup\""<<std::endl;

    _setup_display();
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
        std::this_thread::sleep_for(std::chrono::milliseconds(MS_SLEEP_));

    }
}

void Session::set_metronome_display(std::function<void(int)> metrodisp){
    _notify_metronome_display = metrodisp;
}

void Session::set_disp_initializer(std::function<void(int[8])> di){
    _initialize_display = di;
}

void Session::_update_metronome_display(){
    int metro_state = looper.get_metronome_state();
    if (metro_state!=-1){
        _notify_metronome_display (metro_state);
    }
}

void Session::_setup_display(){
    int diplay_data[9] = {
            (int)cfg.get_button_state(IN1_ARM),
            (int)cfg.get_button_state(IN1_MNTR),
            (int)cfg.get_button_state(IN2_ARM),
            (int)cfg.get_button_state(IN2_MNTR),
            (int)cfg.get_button_state(IN1_EFF1),
            (int)cfg.get_button_state(IN1_EFF2),
            (int)cfg.get_button_state(IN2_EFF1),
            (int)cfg.get_button_state(IN2_EFF2),
            cfg.get_curr_session()
            };

    _initialize_display(diplay_data);
}

void Session::_update_buffers(float *input_buffers[F_NUM_INPUTS],float *output_buffers[F_NUM_OUTPUTS]){

    bool monitorIn[F_NUM_INPUTS]; 
    bool armEnabled[F_NUM_INPUTS];
    monitor.get_states(monitorIn,true);
    monitor.get_states(armEnabled,false); 
    std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> *looper_buff = looper.update_buffer(input_buffers,armEnabled);
    mixer.update_buffer(input_buffers,output_buffers,*looper_buff,monitorIn);

    _update_metronome_display(); 

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

// load config
    std::string current_session = cfg.currSession_name;
    cfg.open(next_session);
    cfg.display();
    std::cout<<"Session::reset : Session changed from \""<<current_session<<"\" to \""<<cfg.currSession_name<<"\""<<std::endl;

    // change_audio_client();
    re_initialize.store(true);

    looper.reset();

// @TODO mixer reset

    setup();
}

void Session::evacuate(){
    is_running.store(false);
    looper.reset();
    monitor.disconnect();
}

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

void Session::notify_session(Trigger trigger, Response &response){

    Control control = trigger.control.load();
    int subvalue  = trigger.subval.load();

    switch (control)
    {
        case CH1_RECDUB:
            looper.recdub(0, (bool)subvalue, response);
            break;
        case CH1_STOP:
            looper.stoperase(0,(bool)subvalue, response);
            break;
        case CH2_RECDUB:
            looper.recdub(1,(bool)subvalue, response);
            break;
        case CH2_STOP:
            looper.stoperase(1,(bool)subvalue, response);
            break;
        case CH3_RECDUB:
            looper.recdub(2,(bool)subvalue, response);
            break;
        case CH3_STOP:
            looper.stoperase(2,(bool)subvalue, response);
            break;
        case START_ALL:
            looper.start_stop_all((bool)subvalue, response); // startALL/stopALL
            break;
        case SAVE_JAM:
            if ((bool)subvalue){
                response.msg.store(JAM_SAVED);
                save_jam();
            }
            break;
        case IN1_ARM:
            response.holder.store(9);
            response.msg.store(ARM);
            response.value.store(monitor.toggle_states(false, 0));
            cfg.button_states[IN1_ARM] = response.value.load();
            // if (cfg.get_button_state(IN1_ARM)){
            //     std::cout<<"Arm for IN1 is ON"<<std::endl;
            // }else std::cout<<"Arm for IN1 is OFF"<<std::endl;
            break;
        case IN1_MNTR:
            response.holder.store(10);
            response.msg.store(MNTR);
            response.value.store(monitor.toggle_states(true, 0));
            cfg.button_states[IN1_MNTR] = response.value.load();
            // if (cfg.get_button_state(IN1_MNTR)){
            //     std::cout<<"Monitor for IN1 is ON"<<std::endl;
            // }else std::cout<<"Monitor for IN1 is OFF"<<std::endl;
            break;
        case IN2_ARM:
            response.holder.store(11);
            response.msg.store(ARM);
            response.value.store(monitor.toggle_states(false, 1));
            cfg.button_states[IN2_ARM] = response.value.load();
            // if (cfg.get_button_state(IN2_ARM)){
            //     std::cout<<"Arm for IN2 is ON"<<std::endl;
            // }else std::cout<<"Arm for IN2 is OFF"<<std::endl;
            break;
        case IN2_MNTR:
            response.holder.store(12);
            response.msg.store(MNTR);
            response.value.store(monitor.toggle_states(true, 1));
            cfg.button_states[IN2_MNTR] = response.value.load();
            // if (cfg.get_button_state(IN2_MNTR)){
            //     std::cout<<"Monitor for IN2 is ON"<<std::endl;
            // }else std::cout<<"Monitor for IN2 is OFF"<<std::endl;
            break;
        case IN1_EFF1:
            response.holder.store(13);
            response.msg.store(EFF);
            response.value.store(monitor.toggle_effect(0,0));
            cfg.button_states[IN1_EFF1] = response.value.load();
            // if (cfg.get_button_state(IN1_EFF1)){
            //     std::cout<<"Eff 1 for IN1 is ON"<<std::endl;
            // }else std::cout<<"Eff 1 for IN1 is OFF"<<std::endl;
            break;
        case IN1_EFF2:
            response.holder.store(14);
            response.msg.store(EFF);
            response.value.store(monitor.toggle_effect(0,1));
            cfg.button_states[IN1_EFF2] = response.value.load();
            // if (cfg.get_button_state(IN1_EFF2)){
            //     std::cout<<"Effect 2 for IN1 is ON"<<std::endl;
            // }else std::cout<<"Effect 2 for IN1 is OFF"<<std::endl;
            break;
        case IN1_EFF3:
            // response.holder.store(15);
            // response.msg.store(EFF);
            // response.value.store(monitor.toggle_effect(0,2));
            // cfg.button_states[IN1_EFF3] = response.value.load();
            // if (cfg.get_button_state(IN1_EFF3)){
            //     std::cout<<"Effect 3 for IN1 is ON"<<std::endl;
            // }else std::cout<<"Effect 3 for IN1 is OFF"<<std::endl;
            break;
        case IN2_EFF1:
            response.holder.store(16);
            response.msg.store(EFF);
            response.value.store(monitor.toggle_effect(1,0));
            cfg.button_states[IN2_EFF1] = response.value.load();
            // if (cfg.get_button_state(IN2_EFF1)){
            //     std::cout<<"Effect 1 for IN2 is ON"<<std::endl;
            // }else std::cout<<"Effect 1 for IN2 is OFF"<<std::endl;
            break;
        case IN2_EFF2:
            response.holder.store(17);
            response.msg.store(EFF);
            response.value.store(monitor.toggle_effect(1,1));
            cfg.button_states[IN2_EFF2] = response.value.load();
            // if (cfg.get_button_state(IN2_EFF2)){
            //     std::cout<<"Effect 2 for IN2 is ON"<<std::endl;
            // }else std::cout<<"Effect 2 for IN2 is OFF"<<std::endl;
            break;
        case IN2_EFF3:
            // response.holder.store(18);
            // response.msg.store(EFF);
            // response.value.store(monitor.toggle_effect(1,2));
            // cfg.button_states[IN2_EFF3] = response.value.load();
            // if (cfg.get_button_state(IN2_EFF3)){
            //     std::cout<<"Effect 3 for IN2 is ON"<<std::endl;
            // }else std::cout<<"Effect 3 for IN2 is OFF"<<std::endl;
            break;
        case TAP_TEMPO:
            looper.tap_alter_metronome((bool)subvalue);
            break;
        case CH1_VOL_LOW:
            looper.volume_change(0,subvalue);
            // looper.volume_down(0);
            break;
        case CH1_VOL_HIGH:
            looper.volume_change(0,subvalue);
            // looper.volume_up(0);
            break;
        case CH2_VOL_LOW:
            looper.volume_change(1,subvalue);
            // looper.volume_down(1);
            break;
        case CH2_VOL_HIGH:
            looper.volume_change(1,subvalue);
            // looper.volume_up(1);
            break;
        case CH3_VOL_LOW:
            looper.volume_change(2,subvalue);
            // looper.volume_down(2);
            break;
        case CH3_VOL_HIGH:
            looper.volume_change(2,subvalue);
            // looper.volume_up(2);
            break;   
    }
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