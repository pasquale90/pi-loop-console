#ifndef ROUTE_H_INCLUDED
#define ROUTE_H_INCLUDED

//######################################################################################################################
//FORWARD DECLARATIONS
//######################################################################################################################
#include <string>
class handshake;
typedef struct _jack_port jack_port_t;
//#####################################################################################################################

struct track_port{
    jack_port_t *input_port;
    jack_port_t *output_port_left;
    jack_port_t *output_port_right;
    jack_port_t *output_port_mono;
};

class route {
public:

    route(handshake *hs);// for monitor
    route(std::string track_name);// for tracks
    ~route();

    ///*ERASE
    jack_port_t* get_input_port() { return tp.input_port; }
    jack_port_t* get_output_port_left() { return tp.output_port_left; }
    jack_port_t* get_output_port_right() { return tp.output_port_right; }
    //*/

    /*track methods for creating audio and midi tracks*/
    /*virtual*/ void add_input(handshake *hs);
    /*virtual*/ void add_stereo(handshake *hs);
    /*virtual*/ void add_mono(handshake *hs);

    /*mute/unmute operations*/
    /*
     * mute each individual track method for pausing if pressed "STOP".STOP should enable user to re enable the connection
     * These methods are to be handled by channel instance , massively for all tracks registered within it.
     */
    void mute_input_port(handshake *hs);//both instrunment and digital
    void mute_stereo_port(handshake *hs);
    void mute_mono_port(handshake *hs);
    void unmute_input_port(handshake *hs);//instrunment
    void unmute_stereo_port(handshake *hs);
    void unmute_mono_port(handshake *hs);

    void connect_all(handshake *hs);
protected:

    /*Encapsulation methods*/
    void set_routename(std::string routename){this->routename=routename;}
    void set_portnames(handshake *hs);//internal process
    track_port get_track_ports() { return tp;}

private:
    track_port tp;
    std::string routename;

};
#endif // ROUTE_H_INCLUDED
