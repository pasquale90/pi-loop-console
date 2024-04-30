#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <functional>
#include "config.h"
#include "interface.h"
#include "session.h"

class Menu {
    public:
        static Menu& getInstance(); //Singleton

        void load();
        void edit(Control);
    
    private:
        //Singleton
        Menu();
        Menu(const Menu&) = delete;
        void operator=(const Menu&) = delete;
        static Menu* menu_instance_ptr;

        Config& cfg = Config::getInstance();
        hardwareInterface& interface=hardwareInterface::getInstance();

        void change_session(Control);
        void save_session();

        void _notify_menu(Control, bool);
        Session session;
};

#endif