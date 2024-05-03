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

#endif
