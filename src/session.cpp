#include "session.h"


Session::Session(){
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
    // ...

}

bool Session::migrate(int next_session){
// Session::migrate is responsible for loading all session data

// load config
    std::string current_session = cfg.currSession_name;
    cfg.open(next_session);
    cfg.display();
    std::cout<<"Session::reset : Session changed from \""<<current_session<<"\" to \""<<cfg.currSession_name<<"\""<<std::endl;   

// load other session data
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

    if (trigger==START_ALL && isHold == 1){
        reset2defaults();
    }
}


