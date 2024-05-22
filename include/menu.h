#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "session.h"

class Menu {
    public:
        static Menu& getInstance(); //Singleton

        void load_session();
        void notify_menu(Control, bool);
        void unload();
    
    private:
        //Singleton
        Menu();
        Menu(const Menu&) = delete;
        void operator=(const Menu&) = delete;
        static Menu* menu_instance_ptr;

        Config& cfg = Config::getInstance();

        void _edit(Control);
        void _change_session(Control);
        void _save_session();

        Session session;
};

#endif