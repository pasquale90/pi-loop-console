#ifndef SESSION_H_INCLUDED
#define SESSION_H_INCLUDED


class Session{
    public:
        Session(const char*);
        // ~Session();

        const char* get_name();
    private:

        const char* session_name;

};

#endif