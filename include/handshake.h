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

// #include <thread>
#include <atomic>

                    #include <chrono>
#include <jack/jack.h>
#include "config.h"

static const float cpuLoad_thress = 0.8;

// @TODO create status control for client -> use it within connect() member function.
struct {
    bool samplingRate;
    bool bufferSize;
    bool isRealTime;
    float cpuload;
    float latency_ms;
    int link_client;
    int set_process_callback;
    int jack_set_buffer_size;
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

        // void start_server();
 
        void setup(); // conforms to the config file for current channel --> channel
  
        // void disconnect_input_device();
        // void disconnect_output_device();
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

        static void jack_shutdown(void*);
        jack_port_t* register_input_port(const char*);
        jack_port_t* register_output_port(const char*);
        
        char* _reset_client_name();
        int link_client();
        int set_process_callback();
        void prevent_failure();
        bool activate();

        Config& cfg = Config::getInstance();
        // std::thread clientThread;

        std::atomic<bool> is_running,is_firsTime,reinitialization;
        
        void connect(bool verbose = false);
        void disconnect();
        
        void connect_input_device(int, const char*);
        void connect_output_device(int, const char*);
        bool register_devices();

        uint32_t get_sample_rate ();
        uint32_t get_buffer_size ();
        float get_cpu_load ();
        bool realTime_enabled ();

        // other 
        bool verbose;
        void info_control();
        void check_status();
};

#endif
