#ifndef SESSION_H_INCLUDED
#define SESSION_H_INCLUDED

#include <iostream>

enum Control: unsigned short;

class Session{
    public:
        Session();
        Session(const char*);
// Comply to the rule of 3/5
        //~Session(); 

        void set_name(const char*);
        const char* get_name();

        void notify_session(Control, bool);
    private:
        const char* session_name;

};

#endif