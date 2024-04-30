#ifndef SESSION_H_INCLUDED
#define SESSION_H_INCLUDED


class Session{
    public:
        Session();
        Session(const char*);
// Comply to the rule of 3/5
        //~Session(); 

        void set_name(const char*);
        const char* get_name();
    private:

        const char* session_name;

};

#endif