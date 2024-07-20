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

/*! @brief The jack-audio client.*/
class Handshake{
  public:
        
        /*! @brief Class contructor.
        */
        Handshake();  

        /*! @brief Function to open the jack client
         * @param char* - the client name
         * @return void
        */
        void link_client(char*);

        /*! @brief Function to set the callback function used for streaming the audio signal
         * @return void
        */
        void set_process_callback();

        /*! @brief Function to set another function to be called in case the system fails.
         * @return void
        */
        void prevent_failure(); 

        /*! @brief Function to register a new input port.
         * @param int - index of the input port.
         * @return void
        */      
        void register_input_port(int);

        /*! @brief Function to register a new output port.
         * @param int - index of the output port.
         * @return void
        */  
        void register_output_port(int);

        /*! @brief Function to activate the jack client 
         * @return void
        */  
        void activate();

        /*! @brief Function to register the input and the output devices.
         * @return void
        */  
        void register_devices();

        /*! @brief Function to change the buffer size using the preprocessor derivative used in audio_settings.h
         * @return void
        */  
        void set_buffer_size();

        /*! @brief Function to check the status of the client connection.
         * @return void
        */  
        void check_status();

        /*! @brief Function to disconnect the client from the server.
         * @return void
        */  
        void disconnect_client();

        /*! @brief Function to connect input ports with input devices
         * @param int - index of the input port.
         * @return void
        */  
        void connect_input_device(int);

        /*! @brief Function to connect output ports with output devices
         * @param int - index of the output port.
         * @return void
        */    
        void connect_output_device(int);

        /*! @brief Function to disconnect input ports from the connected input devices
         * @param int - index of the input port.
         * @return void
        */  
        void disconnect_input_device(int);
        
        /*! @brief Function to disconnect output ports from the connected output devices
         * @param int - index of the output port.
         * @return void
        */  
        void disconnect_output_device(int);


        /*! @brief Function to get the input buffer from a specific port-device.
         * @param int - index of the input port.
         * @return float* - the input buffer 
        */
        float* get_input_buffer(int);


        /*! @brief Function to get the output buffer to which the output port-device is connected.
         * @param int - index of the output port.
         * @return float* - the output buffer where the output signal is written to.
        */  
        float* get_output_buffer(int);

        /*! @brief Function to get the input buffer from a specific port-device.
         * @param int - index of the input port.
         * @param float* - the input buffer passed by value.
         * @return void
        */
        void get_input_buffer(int,float*);

        /*! @brief Function to get the output buffer to which the output port-device is connected.
         * @param int - index of the output port.
         * @param float* - the output buffer passed by value.
         * @return void
        */  
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


        /*! @brief Function be called in case the audio server/client fails.
         * @param void* - args
         * @return void
        */  
        static void _jack_shutdown(void*); // NEED TO BE STATIC?


    // additional verbose functions
        /*! @brief Function to return the value of the sampling rate defined
         * @return uint32_t - the sampling rate
        */  
        uint32_t _get_sample_rate ();

        /*! @brief Function to return the value of the buffer size
         * @return uint32_t - the buffer size
        */  
        uint32_t _get_buffer_size ();

        /*! @brief Function to return the cpu load using jack api
         * @return float -  the cpu load percentage
        */  
        float _get_cpu_load ();

        /*! @brief Function to return if the real-time option is enabled.
         * @return bool - 1 if enabled, 0 if not.
        */  
        bool _realTime_enabled ();

        /*! @brief Function to provide info on various internall operations
         * @return void
        */  
        void _info_control();
};

#endif
