#ifndef HANDSHAKE_H_INCLUDED
#define HANDSHAKE_H_INCLUDED

// jack audio operations
/** @file simple_client.c
 *
 * @brief This simple client demonstrates the most basic features of JACK
 * as they would be used by many applications.
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <jack/jack.h>



class AudioStream{

public:

  void run_server(int,char*[]);
    
    int process(jack_nframes_t);
  
private:

    jack_port_t *input_port;
    jack_port_t *output_port;
    jack_client_t *client;

};


/*
class AudioStream{
public:

    AudioStream(const char*,const char*); 
    
    // void start_server();
    void connect_client();

    void AudioRouting();

    void closeStream();

    int streamBuffer(jack_nframes_t nframes);
    
private:
    jack_port_t *input_port1;
    // jack_port_t *input_port2;
    jack_port_t *output_port_left;
    jack_port_t *output_port_right;
    jack_client_t *client;

    jack_default_audio_sample_t *in,*out;
    const char *server_name;
    const char *client_name ;
    const char **fromdevice;
    const char **todevice;

    static void jack_shutdown (void *arg);
    void mix();
};

*/
#endif
