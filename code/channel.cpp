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
    io=new handshake(chname);

    direct=new route(io);

    io->callback(process,0);//inside callback, monitor and looped samples are included | Process, a global function
    io->prevent_failure();

    io->activate();

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
