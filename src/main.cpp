#include <iostream>
#include <thread>
#include "menu.h"


// callback_process()


#include "handshake.h"

int main(int argc, char *argv[]){

    std::cout<<"Hello Piloop"<<std::endl;

/*
    Menu& menu = Menu::getInstance();
    
//calls menu.load() 
    // menu.load();
    std::thread menuThread(&Menu::load, &menu);
    menuThread.join();
// handle session menu
    // menu.change_session();
*/

    AudioStream as;
    as.run_server(argc,argv);
    // AudioStream as("TestClient",NULL);
    // as.AudioRouting();
    // as.streamBuffer(512);
    // as.closeStream();

    
// initialize a session
// start console

    return 0;
}