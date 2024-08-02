#include "piloop.h"

PiLoop::PiLoop(){

    std::cout<<"Hello Piloop"<<std::endl;    

    menu.set_metro_display(std::bind(&hardwareInterface::get_metro_display,&interface, std::placeholders::_1) );
    menu.set_display_initializer(std::bind(&hardwareInterface::get_display_initializer,&interface, std::placeholders::_1) );
    
    serverThread = std::thread(&AudioServer::start, &server);
    uiThread = std::thread(&PiLoop::_listen, this);
    sessionThread = std::thread(&Menu::load_session,&menu);
    displayThread = std::thread(&hardwareInterface::display,&interface);
}

void PiLoop::start_console(){

    menu.setup();

    serverThread.join();
    uiThread.join();
    sessionThread.join();
    displayThread.join();
}

void PiLoop::shutdown(){
    menu.unload();
    interface.deafen();
    server.stop();
    exit(0);
}

void PiLoop::_listen()
{
    interface.listen(&PiLoop::_notify, *this);
}

void PiLoop::_notify(Trigger trigger){

    Response response;

    if(trigger.control.load() == SHUTDOWN_PILOOP){
        response.msg.store(SHUTDOWN);
        interface.update(response);
        shutdown();
    }
    else{

        if (trigger.control.load() == SAVE_JAM ){
            response.msg.store(JAM_SAVED);
            response.value.store(0);
            interface.update(response);
        }

        menu.notify_menu(trigger,response);
    }
    
    if (!response.isEmpty() || response.looper_state.is_changed.load())
        interface.update(response);
}