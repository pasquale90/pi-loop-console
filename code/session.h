#ifndef SESSION_H_INCLUDED
#define SESSION_H_INCLUDED

//FORWARD DECLARATION
class channel;

class session{
public:
    session();
    session(const char* session_name);//session_name:get date&time for making unique_session_name
    ~session();

    void save();
    void load();
    void wipe();

    channel* get_channel(){return ch1;}

private:
    const char* session_name;
    channel *ch1;
};


#endif // SESSION_H_INCLUDED
