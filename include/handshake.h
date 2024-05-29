#ifndef HANDSHAKE_H_INCLUDED
#define HANDSHAKE_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <jack/jack.h>

#include "audio_settings.h"

// @TODO ERASE struct latter
struct {
    bool samplingRate {false};
    bool bufferSize {false};
    bool isRealTime {false};
    float cpuload {0.};
    float latency_ms {0.};
    int link_client {-1};
    int set_process_callback {-1};
    int set_buffer_size {-1};
    bool activate {false};
    bool register_devices {false};
//     bool client_connected;
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
        // Handshake (const Handshake&) = delete;
        // Handshake& operator= (const Handshake&) = delete;

        void link_client(char*);
        void set_process_callback();
        void prevent_failure();        
        void register_input_port(int);
        void register_output_port(int);
        void activate();
        void register_devices();
        void set_buffer_size();
        void check_status();

        void disconnect_client();

        void connect_input_device(int);
        void connect_output_device(int);
        void disconnect_input_device(int);
        void disconnect_output_device(int);

        float* get_input_buffer(int);
        float* get_output_buffer(int);
        void get_input_buffer(int,float*);
        void get_output_buffer(int,float*);
  private:
        char *client_name;
        const char *server_name;
        const char **from_device,**to_device;
        jack_port_t *input_ports[F_NUM_INPUTS];
        jack_port_t *output_ports[F_NUM_OUTPUTS];
        jack_client_t *client;

        // inline const char* _concat_chars(const char*,const char*);
        // char* _reset_client_name();
// NEED TO BE STATIC?
        static void _jack_shutdown(void*);

        // additional verbose functions
        uint32_t _get_sample_rate ();
        uint32_t _get_buffer_size ();
        float _get_cpu_load ();
        bool _realTime_enabled ();

        // other 
        void _info_control();
};

#endif
