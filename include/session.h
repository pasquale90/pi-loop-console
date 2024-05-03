#ifndef SESSION_H_INCLUDED
#define SESSION_H_INCLUDED

#include <iostream>
#include "config.h"
#include "controls.h"
// enum Control: unsigned short;


class Session{
    public:
        Session();
        
        void save();
        bool migrate(int);
        void reset2defaults();
        void set_name(const char*);
        const char* get_name();

        void notify_session(Control, bool);
    private:
        
        Config& cfg = Config::getInstance();

};

#endif