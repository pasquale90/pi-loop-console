#ifndef PILOOP_H
#define PILOOP_H

#include <thread>

#include <memory>

#include "interface.h"
#include "menu.h"
#include "audioserver.h"

class PiLoop{

    public:

        PiLoop();
        void start_console();

    private:
    
        AudioServer server;        
        Menu& menu = Menu::getInstance();    
        hardwareInterface& interface=hardwareInterface::getInstance();

        std::thread uiThread;
        std::thread serverThread; 
        std::thread sessionThread; 
        std::thread metroThread;
        
        void _listen();
        void _notify(Control, bool);
        void shutdown();
};

#endif

