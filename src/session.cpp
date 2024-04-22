#include "session.h"
#include "channel.h"//Bump -eraseeee
#include <iostream>

session::session(){}

session::session(const char* session_name){
    this->session_name=session_name;
    ch1=new channel();//creates a default channel.
}

session::~session(){
    free(ch1);
}

void session::save(){}

void session::load(){}
void session::wipe(){}
