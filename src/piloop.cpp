#include "piloop.h"

PiLoop::PiLoop(){
    std::cout<<"Hello Piloop"<<std::endl;
    serverThread = std::thread(&AudioServer::start, &server);
    uiThread = std::thread(&PiLoop::_listen, this);
    sessionThread = std::thread(&Menu::load_session,&menu);
}

void PiLoop::start_console(){
    // cfg.display();
    serverThread.join();
    uiThread.join();
    
    // std::cout<<"Waiting for 3 .................................................."<<std::endl;
    // sleep(3);
    // std::cout<<"Waking up after 3 .................................................."<<std::endl;

    sessionThread.join();
}

void PiLoop::shutdown(){
    menu.unload();
    interface.deafen();
    server.stop();
    // serverThread.detach();
    // uiThread.detach();
    exit(0);
}

void PiLoop::_listen()
{
    interface.listen(&PiLoop::_notify, *this);
}

void PiLoop::_notify(Control trigger, bool isHold){

    // if(trigger == SHUTDOWN_PILOOP && isHold){
    if(trigger == SHUTDOWN_PILOOP){
        shutdown();   
    }
    else
        menu.notify_menu(trigger,isHold);
}