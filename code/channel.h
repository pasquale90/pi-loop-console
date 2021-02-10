#ifndef CHANNEL_H_INCLUDED
#define CHANNEL_H_INCLUDED
/*user aspect:initializes monitor, and registers tracks*/

//######################################################################################################################
//FORWARD DECLARATIONS
//######################################################################################################################
class handshake;
class route;
#include <stdint.h>
//#####################################################################################################################

class channel{
public:

    channel();
    ~channel();

    /*DIRECT SIGNAL*/
    int monitor(uint32_t nframes);
    void mute_monitor(); //pause client->which means that have to disconnect and reconnect all "tracks" connected in that client
    void unmute_monitor();//reconnect all the "tracks"->call handshake's "connect_input/_output ports"

    /*REGISTER TRACKS*/
    void mute_channel();
    void unmute_channel();

    /*LOOPER*/
    void record();//loop and overdub
    void stop();//locks track
    void playback();//playback channel's tracklist
    void undo();//delete freshly recorded track

    //CLASS INTERNAL METHODS
    void disconnect_channel();

    handshake* get_IO(){return io;}

private:
    const char* chname;
    //connection related
    handshake *io;
    route *direct;//embedd into audio

    //vector<vector<float>> tracklist;  || vector<track> tracklist tracklist;


};

#endif // CHANNEL_H_INCLUDED
