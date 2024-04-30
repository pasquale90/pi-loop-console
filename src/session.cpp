#include "session.h"


Session::Session(const char* sessionName):session_name(sessionName) {
    // ...   
}

const char* Session::get_name(){
    return session_name;
}