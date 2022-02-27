#include "channel.h"
#include "handshake.h"
#include "route.h"
#include <iostream>
#include <string>
#include <cstring>

extern "C" void *jack_port_get_buffer(jack_port_t *, jack_nframes_t);
int process (jack_nframes_t  nframes,void *arg);

channel::channel(){
    static int chid=1;
    std::string cn="CH"+std::to_string(chid);   //sets client name to "CH{#channel}"
    chname=cn.c_str();

std::cout<<"DEBUG Segmentation fault:: channel_constructor:: creating a channel"<<std::endl;
    io=new handshake(chname);
std::cout<<"DEBUG Segmentation fault:: channel_constructor:: created a handshake"<<std::endl;

std::cout<<"DEBUG Segmentation fault:: channel_constructor:: creating a route"<<std::endl;
    direct=new route(io);

std::cout<<"DEBUG Segmentation fault:: channel_constructor:: callback"<<std::endl;
    io->callback(process,0);//inside callback, monitor and looped samples are included | Process, a global function

std::cout<<"DEBUG Segmentation fault:: channel_constructor::prevent_failure"<<std::endl;
    io->prevent_failure();

std::cout<<"DEBUG Segmentation fault:: channel_constructor::activate"<<std::endl;
    io->activate();

std::cout<<"DEBUG Segmentation fault:: channel_constructor::connect_all"<<std::endl;
    direct->connect_all(io);


    ++chid;
}

channel::~channel(){}

//########################################################################################################################
/*track methods for creating audio and midi tracks*/
//########################################################################################################################

void channel::mute_monitor(){
    direct->mute_input_port(io);
    direct->mute_stereo_port(io);
}

void channel::unmute_monitor(){
    direct->connect_all(io);
}

int channel::monitor(uint32_t nframes){
    float *in,*left,*right;
    in = (float *)jack_port_get_buffer (direct->get_input_port(), nframes);
    left = (float *)jack_port_get_buffer (direct->get_output_port_left(), nframes);
    right= (float *)jack_port_get_buffer(direct->get_output_port_right(), nframes);
    std::memcpy (left, in, sizeof (float) *nframes);
    std::memcpy (right, in, sizeof (float) *nframes);
    return 0;
}

//########################################################################################################################
/*track methods for creating audio and midi tracks*/
//########################################################################################################################

void record(){}//loop and overdub
void stop(){}//locks track
void playback(){}//playback channel's tracklist
void undo(){}//delete freshly recorded track

//########################################################################################################################
/*class internal methods*/
//########################################################################################################################

void channel::disconnect_channel(){
    io->disconnect_client();
}
