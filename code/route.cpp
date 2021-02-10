#include "route.h"
#include "handshake.h"
#include <iostream>
#include <stdint.h>

int jack_port_set_name(jack_port_t *port, const char *port_name);
const char *jack_port_name (const jack_port_t *port);

/*channel monitor constructor*/
route::route(handshake *hs){
    this->routename="MONITOR";

    add_input(hs);
    add_stereo(hs);//ADD CONDITION ABOUT CHECKING THE HARDWARE SPECIFICATIONS
}
/*track constructor*/
route::route(std::string track_name){
    static int trackcount=1;

    track_name+=std::to_string(trackcount);
    this->routename=track_name.c_str();
    ++trackcount;

}

//########################################################################################################################
/*track methods for creating audio and midi tracks*/
//########################################################################################################################

void route::add_input(handshake *hs){
    tp.input_port=hs->add_input_port(tp.input_port,(routename+"i").c_str());
}

void route::add_stereo(handshake *hs){
    tp.output_port_left=hs->add_output_port(tp.output_port_left,(routename+"l").c_str());
    tp.output_port_right=hs->add_output_port(tp.output_port_right,(routename+"r").c_str());
    tp.output_port_mono=NULL;
}

void route::add_mono(handshake *hs){
    tp.output_port_mono=hs->add_output_port(tp.output_port_mono,(routename+"o").c_str());
    tp.output_port_left=NULL;
    tp.output_port_right=NULL;
}

//########################################################################################################################
/*mute/unmute route operations*/
//########################################################################################################################

void route::mute_input_port(handshake *hs){
    hs->disconnect_port(tp.input_port);
}

void route::mute_stereo_port(handshake *hs){
    hs->disconnect_port(tp.output_port_left);
    hs->disconnect_port(tp.output_port_right);
}

void route::mute_mono_port(handshake *hs){
    hs->disconnect_port(tp.output_port_mono);
}

void route::unmute_input_port(handshake *hs){
    add_input(hs);
}

void route::unmute_stereo_port(handshake *hs){
    add_stereo(hs);
}

void route::unmute_mono_port(handshake *hs){
    add_mono(hs);
}

//########################################################################################################################
/*internal process*/
//########################################################################################################################

void route::connect_all(handshake *hs){
    if (tp.input_port!=NULL){
        hs->connect_input_device(0,tp.input_port);
    }
    if (tp.output_port_left!=NULL && tp.output_port_right!=NULL){
        hs->connect_output_device(0,tp.output_port_left);
        hs->connect_output_device(1,tp.output_port_right);
    }
    if (tp.output_port_mono!=NULL){
        hs->connect_output_device(0,tp.output_port_mono);
    }
}

