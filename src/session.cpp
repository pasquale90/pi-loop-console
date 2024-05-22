#include "session.h"

Session::Session(){
    // server start
    // load()
    // sleep(3);
    // hs.connect();
}

void Session::save(){
// Session::save is responsible for 
//    a) saving config
//    b) saving session (tracks in channels , saved jams ..)

// save config
    // try{
        cfg.save();
        std::cout<<"Menu::save_session"<<std::endl;
    // }
    // catch {return false;};

// save session
    // ...src/session.cpp:4:18

}

void Session::load(){
    hs.setup();
}

bool Session::migrate(int next_session){
// Session::migrate is responsible for loading all session data

// 

// load config
    std::string current_session = cfg.currSession_name;
    cfg.open(next_session);
    cfg.display();
    std::cout<<"Session::reset : Session changed from \""<<current_session<<"\" to \""<<cfg.currSession_name<<"\""<<std::endl;
// load other session data
    // ...

    hs.reInitialize();
}

void Session::evacuate(){
    hs.stop_running();
    // ...
}

void Session::reset2defaults(){
// clean config
    cfg.reset();

// clean other session data 
    // ...
}

const char* Session::get_name(){
    // return session_name;
    return cfg.currSession_name.c_str();
}

void Session::set_name(const char* name){
    cfg.currSession_name = name;
}

void Session::notify_session(Control trigger, bool isHold){
    std::cout<<"Session::_notify_session --> "<<int(trigger)<<", "<<isHold<<std::endl;

    switch (trigger)
    {
        case CH1_RECDUB:
            std::cout<<"Session::_notify_session -->CH1_RECDUB"<<std::endl;
            break;
        case CH1_STOP:
            std::cout<<"Session::_notify_session -->CH1_STOP"<<std::endl;
            break;
        case CH2_RECDUB:
            std::cout<<"Session::_notify_session -->CH2_RECDUB"<<std::endl;
            break;
        case CH2_STOP:
            std::cout<<"Session::_notify_session -->CH2_STOP"<<std::endl;
            break;
        case CH3_RECDUB:
            std::cout<<"Session::_notify_session -->CH3_RECDUB"<<std::endl;
            break;
        case CH3_STOP:
            std::cout<<"Session::_notify_session -->CH3_STOP"<<std::endl;
            break;
        case START_ALL:
            std::cout<<"Session::_notify_session -->START_ALL"<<std::endl;
            break;
        case SAVE_JAM:
            std::cout<<"Session::_notify_session -->SAVE_JAM"<<std::endl;
            break;
        case IN1_ARM:
            std::cout<<"Session::_notify_session -->IN1_ARM"<<std::endl;
            break;
        case IN1_MNTR:
            std::cout<<"Session::_notify_session -->IN1_MNTR"<<std::endl;
// JUST FOR TESTING 
// @TODO save button states and pass them on the conditions defined
            hs.mute_microphone();
            sleep(3);
            hs.unmute_microphone();
            break;
        case IN2_ARM:
            std::cout<<"Session::_notify_session -->IN2_ARM"<<std::endl;
            break;
        case IN2_MNTR:
            std::cout<<"Session::_notify_session -->IN2_MNTR"<<std::endl;
            break;
        case IN1_EFF1:
            std::cout<<"Session::_notify_session -->IN1_EFF1"<<std::endl;
            break;
        case IN1_EFF2:
            std::cout<<"Session::_notify_session -->IN1_EFF2"<<std::endl;
            break;
        case IN1_EFF3:
            std::cout<<"Session::_notify_session -->IN1_EFF3"<<std::endl;
            break;
        case IN2_EFF1:
            std::cout<<"Session::_notify_session -->IN2_EFF1"<<std::endl;
            break;
        case IN2_EFF2:
            std::cout<<"Session::_notify_session -->IN2_EFF2"<<std::endl;
            break;
        case IN2_EFF3:
            std::cout<<"Session::_notify_session -->IN2_EFF3"<<std::endl;
            break;
        case TAP_TEMPO:
            std::cout<<"Session::_notify_session -->TAP_TEMPO"<<std::endl;
            break;
        case CH1_VOL_LOW:
            std::cout<<"Session::_notify_session -->CH1_VOL_LOW"<<std::endl;
            break;
        case CH1_VOL_HIGH:
            std::cout<<"Session::_notify_session -->CH1_VOL_HIGH"<<std::endl;
            break;
        case CH2_VOL_LOW:
            std::cout<<"Session::_notify_session -->CH2_VOL_LOW"<<std::endl;
            break;
        case CH2_VOL_HIGH:
            std::cout<<"Session::_notify_session -->CH2_VOL_HIGH"<<std::endl;
            break;
        case CH3_VOL_LOW:
            std::cout<<"Session::_notify_session -->CH3_VOL_LOW"<<std::endl;
            break;
        case CH3_VOL_HIGH:
            std::cout<<"Session::_notify_session -->CH3_VOL_HIGH"<<std::endl;
            break;
        
    }
    // if (trigger==START_ALL && isHold == 1){
    //     reset2defaults();
    // }

}


