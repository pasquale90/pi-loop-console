#include "session.h"
#include "handshake.h"
#include "channel.h"
#include <iostream>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <Windows.h> //for development purposes
#else
#include <unistd.h>
#endif // _WIN32


channel *ch;

//process function normally should be a channel's member function.
//Since calling function pointers to member methods are not yet succeeded,
//the later function are currently used as global functions.
int process (jack_nframes_t nframes, void* arg)
{
    ch->monitor(nframes);//no need for condition

    /*
     *
     *
        ADD METHODS HERE
                SO AS TO BE PROCESSED BY CHANNEL'S CALLBACK
     *
     *
     */


    return 0;
}

//as well as ...
//jack_shutdown function normally should be a handshake's member function.
void jack_shutdown(void *arg){exit (1);}

int simple_quit = 0;//represents a quit keyboard event.

int main ()
{
//
std::ofstream cout_log ("logs/cout.log");
std::ofstream cerr_log ("logs/cerr.log");

// connect stream buffers
std::streambuf *coutbuf = std::cout.rdbuf();
std::cout.rdbuf(cout_log.rdbuf () );
std::streambuf *cerrbuf = std::cerr.rdbuf();
std::cerr.rdbuf(cerr_log.rdbuf () );


    std::cout<<"Hello Piloop"<<std::endl;
    /*create a new session*/
    session *ses=new session("ThirdJam");

// restore
std::cout.flush ();
std::cout.rdbuf(cerrbuf);

std::cerr.flush ();
std::cerr.rdbuf(cerrbuf);


    std::cout<<"main::new session has been created"<<std::endl;

    /*A small piece of structure demonstration(1): each session hosts a distinct #ofchannels*/
    ch=ses->get_channel();                     //OR simply get_channels ...
                                                        //..as long as rpi(cpu) ..
                                                        //..and hardware(console buttons) ..
                                                        //..are known beforehand.

    /*A small piece of structure demonstration(2): each channel hosts a unique Jack client*/
    std::cout<<"Channel {"<<ch->get_IO()->get_name()<<"} info control : "<<std::endl;;
    ch->get_IO()->info_control();//print channels configuration info

    ///*

    std::cout<<"Press: \n -M for mute \n -U for unmute. "<<std::endl;
    std::string input;
    while(std::cin>>input){
        if (input=="M"){
            ch->mute_monitor();
        }
        if (input=="U"){
            ch->unmute_monitor();
        }
    }

    //*/
        while (!simple_quit)    /* keep running until until we get a quit event */
#ifdef WIN32
                Sleep(1*1000);
#else
                sleep(1);
#endif

        ses->get_channel()->disconnect_channel();
        return 0;
}
