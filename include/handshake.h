#ifndef HANDSHAKE_H_INCLUDED
#define HANDSHAKE_H_INCLUDED

// jack audio operations
/** @file simple_client.c
 *
 * @brief This simple client demonstrates the most basic features of JACK
 * as they would be used by many applications.
 */

#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <atomic>
#include <jack/jack.h>
#include "config.h"

static const float cpuLoad_thress = 0.8;
static const bool VERBOSE = true;

struct {
    bool samplingRate;
    bool bufferSize;
    bool isRealTime;
    float cpuload;
    float latency_ms;
    int link_client;
    int set_process_callback;
    int set_buffer_size;
    bool activate;
    bool register_devices;
    bool client_connected;
}client_status;  

class Handshake{
  public:
        Handshake();
        
// rule of 5
    // Destructor 
        // ~Handshake(); 
    // // Copy Constructor 
    //     Handshake(const Handshake&);
    // // Copy Assignment Operator 
    //     Handshake& operator=(const Handshake&); 
    // // Move Constructor 
    //     Handshake(Handshake&& ) noexcept;
    // // Move Assignment Operator 
    //     Handshake& operator=(Handshake&& other) noexcept ;
// delete for now - we do not need to create a new client. Just call setup each time session changes.
        Handshake (const Handshake&) = delete;
        Handshake& operator= (const Handshake&) = delete;

        
        void reInitialize();
        void stop_running();
 
        void setup(); // conforms to the config file for current channel --> channel

        void mute_microphone();
        void unmute_microphone();
        void mute_instrument();
        void unmute_instrument();

        int process(jack_nframes_t);
  
  private:
      
        char *client_name;
        const char *server_name;
        const char **from_device,**to_device;
        jack_port_t *input_port_mic,*input_port_inst, *output_port_left, *output_port_right;
        jack_client_t *client;

        static void _jack_shutdown(void*);
        jack_port_t* _register_input_port(const char*);
        jack_port_t* _register_output_port(const char*);
        
        char* _reset_client_name();
        int _link_client();
        int _set_process_callback();
        void _prevent_failure();
        bool _activate();

        Config& cfg = Config::getInstance();

        std::atomic<bool> is_running,is_firsTime,reinitialization;
        
        void _connect(bool verbose = VERBOSE);
        void _disconnect();
        
        void _connect_input_device(int, const char*);
        void _connect_output_device(int, const char*);
        bool _register_devices();

        int _set_buffer_size();
        uint32_t _get_sample_rate ();
        uint32_t _get_buffer_size ();
        float _get_cpu_load ();
        bool _realTime_enabled ();

        // other 
        bool verbose;
        void _info_control();
        void _check_status();

        float* _get_mic_buffer();
        float* _get_left_buffer();
        float* _get_right_buffer();
};

#endif
