#include <iostream>
#include <cstring>
#include "track.h"

track::track(){
    std::cout<<"Implement uniqname first"<<std::endl<<std::endl;
}

void track::register_input_ports(jack_client_t *client,unsigned long flag){//const_char

    /* create input ports */

    tp.input_port = jack_port_register(client, "input", JACK_DEFAULT_AUDIO_TYPE, flag, 0);

    if ((tp.input_port == NULL)) {
        std::cerr<<"no more JACK ports available"<<std::endl;
        exit (1);                                         //FIND AN DIFFERENT WAY TO APROACH EXIT(1)
    }
}

void track::register_output_ports(jack_client_t *client,unsigned long flag){
    //tp.output_port_left = jack_port_register (client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    ///
        /* create two ports */

        tp.output_port_left = jack_port_register (client,"output_left", JACK_DEFAULT_AUDIO_TYPE, flag, 0);
        tp.output_port_right = jack_port_register(client,"output_right",JACK_DEFAULT_AUDIO_TYPE, flag, 0);

        if (tp.output_port_left == NULL || tp.output_port_right == NULL) {
            std::cerr<<"no more JACK ports available"<<std::endl;
            exit (1);
        }
}

void track::mute_input_ports(jack_client_t *client){
    //disconnect input
    if(jack_port_disconnect(client,tp.input_port)){
        std::cerr<<"Couldnt disconnect the "<<jack_port_name(tp.input_port)<<" port from the main stream"<<std::endl;}

    std::cout<<"Connection port {"<<jack_port_name(tp.input_port)<<"} status: {"<<jack_port_connected(tp.input_port)<<"."<<std::endl;
}

void track::mute_output_ports(jack_client_t *client){
    //disconnect left port
    if(jack_port_disconnect(client,tp.output_port_left)){
        std::cerr<<"Couldnt disconnect the "<<jack_port_name(tp.output_port_left)
        <<" port from the main stream"<<std::endl;}
    std::cout<<"Connection port {"<<jack_port_name(tp.output_port_left)<<"} status: {"
        <<jack_port_connected(tp.output_port_left)<<"}."<<std::endl;

    //disconnect right port
    if(jack_port_disconnect(client,tp.output_port_right)){
        std::cerr<<"Couldnt disconnect the "<<jack_port_name(tp.output_port_right)
        <<" port from the main stream"<<std::endl;}
    std::cout<<"Connection port {"<<jack_port_name(tp.output_port_right)<<"} status: {"
        <<jack_port_connected(tp.output_port_right)<<"}."<<std::endl;
}

void track::mute(jack_client_t *client){
    mute_input_ports(client);
    mute_output_ports(client);
}

int track::monitor(uint32_t nframes){
    float *in,*left,*right;
    in = (float *)jack_port_get_buffer (tp.input_port, nframes);
    left = (float *)jack_port_get_buffer (tp.output_port_left, nframes);
    right= (float *)jack_port_get_buffer(tp.output_port_right, nframes);
    std::memcpy (left, in, sizeof (float) *nframes);
    std::memcpy (right, in, sizeof (float) *nframes);
    return 0;
}
