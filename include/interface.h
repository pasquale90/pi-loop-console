#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <iostream>

#ifdef DEV
    #include "keyboard.h"
#endif
#ifdef REL
    #include "buttons.h"
#endif

//Observer class
class hardwareInterface{

    public:
        static hardwareInterface& getInstance();
        
        void listen();
        // void help();

    private:
        hardwareInterface();
        hardwareInterface(const hardwareInterface&) = delete;
        void operator=(const hardwareInterface&) = delete;
        static hardwareInterface* interface_instance_ptr;

        UI& ui = UI::getInstance();

        int notify_menu();
        int notify_session();

        std::atomic<int> msg;
        std::atomic<bool> event_occured;
        // std::string event_list[2];

};


#endif