#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "config.h"
#include "interface.h"

class Menu {
    public:
        static Menu& getInstance();

        void load();
        void change_session();
    
    private:
        Menu();
        Menu(const Menu&) = delete;
        void operator=(const Menu&) = delete;

        static Menu* menu_instance_ptr;

        Config& cfg = Config::getInstance();
        hardwareInterface& interface=hardwareInterface::getInstance();
};

#endif