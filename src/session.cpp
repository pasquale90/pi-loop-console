#include "session.h"


Session::Session(){

}

Session::Session(const char* sessionName):session_name(sessionName) {
    // ...   
}

const char* Session::get_name(){
    return session_name;
}

void Session::set_name(const char* name){
    session_name = name;
}