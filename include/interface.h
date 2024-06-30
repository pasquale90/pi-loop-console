#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <iostream>

#ifdef REL
    #include "gpio/buttons.h"
#elif DEV
    #include "keyboard.h"
#endif

class PiLoop;

//Observer class
class hardwareInterface{

    public:
        static hardwareInterface& getInstance();

        // void listen(void (Menu::*_notify_menu)(Control, bool), Menu& m, void (Session::*_notify_session)(Control, bool), Session& s); // ATTEMPT 2
        void listen(void (PiLoop::*_notify)(Control, bool), PiLoop&);

        void deafen();
        // void help();

    private:
        hardwareInterface();
        hardwareInterface(const hardwareInterface&) = delete;
        void operator=(const hardwareInterface&) = delete;
        static hardwareInterface* interface_instance_ptr;

        std::atomic<bool> keep_running;
        UI& ui = UI::getInstance();
//@TODO make msg and array of messages to accept multiple messages at the same simultaneously
        std::atomic<int> msg;
        std::atomic<bool> event_occured;
        // std::string event_list[2];

        void _parse_msg(Control&, bool&);

};


#endif