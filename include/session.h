#ifndef SESSION_H_INCLUDED
#define SESSION_H_INCLUDED

#include "handshake.h" 

class Session{
    public:
        Session();
        
        void load();
        void save();
        void migrate(int);
        void evacuate();
        void notify_session(Control, bool);

        void set_name(const char*);
        const char* get_name();

    private:
        Handshake hs;
        
        Config& cfg = Config::getInstance();

        void reset2defaults();
        
};

#endif