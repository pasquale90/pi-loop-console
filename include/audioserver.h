#ifndef AUDIOSERVER_H
#define AUDIOSERVER_H

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <jack/jack.h>
#include <jack/control.h>
#include <thread>
#include "config.h"

const char supported_driver[5] = "alsa";

class AudioServer{

public:

    AudioServer(const char* driverName = supported_driver);
    
    void start();
    void stop();

private:
    jackctl_server_t *server;
    const JSList *parameters;
    const JSList *drivers;
    const JSList *node_ptr;
    jackctl_sigmask_t *sigmask;
    const char *driver_name;
    
    void setup_server();
    
    void change_server_parameters();
    void change_ALSAdriver_parameters();
    void print_parameters(const JSList*);
    jackctl_driver_t* jackctl_server_get_driver();
    jackctl_parameter_t * jackctl_get_parameter(const JSList* parameters_list,const char *parameter_name);
    void print_driver_info();
    void print_value(union jackctl_parameter_value, jackctl_param_type_t);

    Config& cfg = Config::getInstance();
};

#endif