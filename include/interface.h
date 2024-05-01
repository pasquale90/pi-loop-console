#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <iostream>

#ifdef DEV
    #include "keyboard.h"
#endif
#ifdef REL
    #include "buttons.h"
#endif

class Menu;
class Session;

//forward declare pointers to member functions
// class Menu;
// // class Session<;
// int (Menu::*notify_menu)() = NULL;
// int (TMyClass::*pt2ConstMember)(float, char, char) const = NULL;

//Observer class
class hardwareInterface{

    public:
        static hardwareInterface& getInstance();

        void listen(void (Menu::*_notify_menu)(Control, bool), Menu& m, void (Session::*_notify_session)(Control, bool), Session& s); // ATTEMPT 2
        // void help();

    private:
        hardwareInterface();
        hardwareInterface(const hardwareInterface&) = delete;
        void operator=(const hardwareInterface&) = delete;
        static hardwareInterface* interface_instance_ptr;

        UI& ui = UI::getInstance();

        std::atomic<int> msg;
        std::atomic<bool> event_occured;
        // std::string event_list[2];

        void parse_msg(Control&, bool&);

};


#endif