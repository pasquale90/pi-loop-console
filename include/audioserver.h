#ifndef AUDIOSERVER_H
#define AUDIOSERVER_H

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <jack/jack.h>
#include <jack/control.h>

const char supported_driver[5] = "alsa";

/*! @brief The jack-audio server running on the alsa drivers*/
class AudioServer{

public:
    /*! @brief Default constructor
    */
    AudioServer(const char* driverName = supported_driver);
    
    /*! @brief Starts the jack audio server
    * @return void
    */
    void start();

    /*! @brief Stops the jack audio server
    * @return void
    */
    void stop();

private:
    jackctl_server_t *server;
    const JSList *parameters;
    const JSList *drivers;
    const JSList *node_ptr;
    jackctl_sigmask_t *sigmask;
    const char *driver_name;
    
    /*! @brief Setup the jack audio server by changing server parameters and alsa driver parameters
    * @return void
    */
    void setup_server();
    
    /*! @brief Function to change the jack audio server parameters
    * @return void
    */
    void change_server_parameters();

    /*! @brief Function to change the alsa driver parameters
    * @return void
    */
    void change_ALSAdriver_parameters();

    /*! @brief Function that prints server and driver parameters and can be used for configuring both server and driver on setup.
    * @param const JSList* - a pointer to a JSList that holds a table of all configurable parameters.
    * @return void
    */
    void print_parameters(const JSList*);

    /*! @brief Function to get the selected driver. In this program, the alsa driver is selected by default.
    * @return jackctl_driver_t* -  a pointer to the driver instance.
    */
    jackctl_driver_t* jackctl_server_get_driver();

    /*! @brief helper function used to get a pointer to a configurable parameter.
    * @param const JSList* - the parameters_list
    * @param const char* - the parameter's name
    * @return jackctl_parameter_t* - a pointer to a the selected parameter object
    */
    jackctl_parameter_t* jackctl_get_parameter(const JSList*,const char *);

    /*! @brief helper function used to print all the available information relate to the driver of interest. 
    * In this case, parameters of the ALSA driver
    * @return void
    */
    void print_driver_info();

    /*! @brief helper function to print the values of parameters.
    * @param union jackctl_parameter_value -  Type for parameter value, intentionally similar to jack_driver_param_value_t
    * @param jackctl_param_type_t - Parameter types, intentionally similar to jack_driver_param_type_t
    * @return void
    */
    void print_value(union jackctl_parameter_value, jackctl_param_type_t);

};

#endif