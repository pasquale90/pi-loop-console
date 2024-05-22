#include "audioserver.h"


// AudioServer::AudioServer(){
//     driver_name=driver;
//     server = jackctl_server_create2(NULL, NULL, NULL);
//     parameters = jackctl_server_get_parameters(server);
//     sigmask = jackctl_setup_signals(0);

// }

AudioServer::AudioServer(const char* driverName):driver_name(driverName){
    server = jackctl_server_create2(NULL, NULL, NULL);
    parameters = jackctl_server_get_parameters(server);
    sigmask = jackctl_setup_signals(0);
}

void AudioServer::stop(){
    printf("\n\nShutting down server\n\n");
    jackctl_server_stop(server);
    jackctl_server_close(server);
    jackctl_server_destroy(server);
}

void AudioServer::setup_server(){

//config here??
    change_server_parameters();
    print_driver_info();
    change_ALSAdriver_parameters();

}

void AudioServer::start(){

    setup_server();

    jackctl_server_open(server, jackctl_server_get_driver());
    jackctl_server_start(server);
    jackctl_wait_signals(sigmask);
}


void AudioServer::change_server_parameters(){
    // printf("\n========================== \n");
    // printf("List of server parameters \n");
    // printf("========================== \n");    
    // print_parameters(parameters);
// print_parameters(parameters) output below...
    // parameter name = name
    // parameter id = 
    // parameter short decs = Server name to use.
    // parameter long decs = 
    // parameter value = default

    // parameter name = realtime
    // parameter id = 
    // parameter short decs = Whether to use realtime mode.
    // parameter long decs = Use realtime scheduling. This is needed for reliable low-latency performance. On most systems, it requires JACK to run with special scheduler and memory allocation privileges, which may be obtained in several ways. On Linux you should use PAM.
    // parameter value = 1

    // parameter name = realtime-priority
    // parameter id = 
    // parameter short decs = Scheduler priority when running in realtime mode.
    // parameter long decs = 
    // parameter value = 10

    // parameter name = temporary
    // parameter id = 
    // parameter short decs = Exit once all clients have closed their connections.
    // parameter long decs = 
    // parameter value = 0

    // parameter name = verbose
    // parameter id = 
    // parameter short decs = Verbose mode.
    // parameter long decs = 
    // parameter value = 0

    // parameter name = client-timeout
    // parameter id = 
    // parameter short decs = Client timeout limit in milliseconds.
    // parameter long decs = 
    // parameter value = 0

    // parameter name = clock-source
    // parameter id = 
    // parameter short decs = Clocksource type : c(ycle) | h(pet) | s(ystem).
    // parameter long decs = 
    // parameter value = 0

    // parameter name = port-max
    // parameter id = 
    // parameter short decs = Maximum number of ports.
    // parameter long decs = 
    // parameter value = 2048

    // parameter name = replace-registry
    // parameter id = 
    // parameter short decs = Replace shared memory registry.
    // parameter long decs = 
    // parameter value = 0

    // parameter name = sync
    // parameter id = 
    // parameter short decs = Use server synchronous mode.
    // parameter long decs = 
    // parameter value = 0

    // parameter name = self-connect-mode
    // parameter id = 
    // parameter short decs = Self connect mode.
    // parameter long decs = Whether JACK clients are allowed to connect their own ports
    // parameter value =  

    // change server param --> make verbose
    jackctl_parameter_t* param;
    union jackctl_parameter_value value;
    param = jackctl_get_parameter(parameters, "verbose");
    if (param != NULL) {
        value.b = false;//true;
        jackctl_parameter_set_value(param, &value);
    }
    // change server param --> make real-time
    param = jackctl_get_parameter(parameters, "realtime");
    if (param != NULL) {
        value.b = true;
        jackctl_parameter_set_value(param, &value);
        printf("Success on changing real time");
    }

    // change server param --> change real-time priority
    param = jackctl_get_parameter(parameters, "realtime-priority");
    if (param != NULL) {
        value.b = 80;
        jackctl_parameter_set_value(param, &value);
        printf("Success on changing real-time priority");
    }
}

void AudioServer::print_parameters(const JSList * node_ptr)
{
    while (node_ptr != NULL) {
        jackctl_parameter_t * parameter = (jackctl_parameter_t *)node_ptr->data;
        printf("\nparameter name = %s\n", jackctl_parameter_get_name(parameter));
        printf("parameter id = %c\n", jackctl_parameter_get_id(parameter));
        printf("parameter short decs = %s\n", jackctl_parameter_get_short_description(parameter));
        printf("parameter long decs = %s\n", jackctl_parameter_get_long_description(parameter));
        print_value(jackctl_parameter_get_default_value(parameter), jackctl_parameter_get_type(parameter));
        node_ptr = jack_slist_next(node_ptr);
    }
}

jackctl_driver_t* AudioServer::jackctl_server_get_driver()
{
    const JSList * node_ptr = jackctl_server_get_drivers_list(server);
    while (node_ptr) {
        if (strcmp(jackctl_driver_get_name((jackctl_driver_t *)node_ptr->data), driver_name) == 0) {
            return (jackctl_driver_t *)node_ptr->data;
        }
        node_ptr = jack_slist_next(node_ptr);
    }
    return NULL;
}

jackctl_parameter_t* AudioServer::jackctl_get_parameter(const JSList * parameters_list,const char * parameter_name){
    while (parameters_list)
    {
        if (strcmp(jackctl_parameter_get_name((jackctl_parameter_t *)parameters_list->data), parameter_name) == 0)
        {
            return (jackctl_parameter_t *)parameters_list->data;
        }
        parameters_list = jack_slist_next(parameters_list);
    }
    return NULL;
}

void AudioServer::print_value(union jackctl_parameter_value value, jackctl_param_type_t type){
    switch (type) {
    
        case JackParamInt:
            printf("parameter value = %d\n", value.i);
            break;
            
         case JackParamUInt:
            printf("parameter value = %u\n", value.ui);
            break;
            
         case JackParamChar:
            printf("parameter value = %c\n", value.c);
            break;
        
         case JackParamString:
            printf("parameter value = %s\n", value.str);
            break;
            
         case JackParamBool:
            printf("parameter value = %d\n", value.b);
            break;
     }
}

void AudioServer::print_driver_info(){
    const JSList * drivers = jackctl_server_get_drivers_list(server);
    const JSList * node_ptr = drivers;
    while (node_ptr != NULL) {
        jackctl_driver_t *driver = (jackctl_driver_t *)node_ptr->data;
        if (!strcmp(jackctl_driver_get_name(driver),driver_name)){
            printf("\n--------------------------\n");
            printf("driver = %s\n", jackctl_driver_get_name(driver));
            printf("-------------------------- \n");
            print_parameters(jackctl_driver_get_parameters(driver)); 
        }
        node_ptr = jack_slist_next(node_ptr);
    }    
}

inline const char* _concat_chars(const char* ch1, const char* ch2){
    int s1 = sizeof(ch1)/sizeof(ch1[0]);
    int s2 = sizeof(ch2)/sizeof(ch2[0]);
    char *s = new char[s1+s2+1];
    // char s[s1+s2+1];
    strcpy(s,ch1);
    strcat(s,ch2);
    return s;
}

void AudioServer::change_ALSAdriver_parameters(){
    
    const JSList * drivers = jackctl_server_get_drivers_list(server);
    const JSList * node_ptr = drivers;
    while (node_ptr != NULL) {
        
        jackctl_driver_t *driver = (jackctl_driver_t *)node_ptr->data;
        if (!strcmp(jackctl_driver_get_name(driver),driver_name)){ // change device to hw:PCH

                    const JSList * param_ptr = jackctl_driver_get_parameters(driver);

                    while (param_ptr != NULL) {
                        
                        jackctl_parameter_t * parameter = (jackctl_parameter_t *)param_ptr->data;
                        const char* param_name = jackctl_parameter_get_name(parameter);
// Config::sampling rate
                        if (!strcmp(param_name,"rate")){
                            int sr = cfg.audio_settings.sample_rate;
                            bool SRISCHANGED = jackctl_parameter_set_value (parameter, (const union jackctl_parameter_value*)&sr);
                        }
// Config::device
                        else if (!strcmp(param_name,"device")){
                            // const char* device_name = "hw:PCH";
                            const char* device_name = _concat_chars("hw:",cfg.device_settings.audio_device.c_str());
// cfg.device_settings.
// char *s = new[strlen("hw:")+strlen()+1];
// strcpy(s,word);
// strcat(s,word2);     
                            if (jackctl_parameter_set_value (parameter, (const union jackctl_parameter_value*)device_name )){
                                printf("device name has changed!");
                            }
                            else{
                                printf("device name has NOT changed!");
                            }
                        }
// Config::buffer size
                        else if (!strcmp(param_name,"period")){
                            int buffer_size = cfg.audio_settings.buffer_size;
                            if (jackctl_parameter_set_value (parameter, (const union jackctl_parameter_value*)&buffer_size)){
                                printf("buffer size has changed!");
                            }
                            else{
                                printf("buffer size has NOT changed!");
                            }
                        }
                        param_ptr = jack_slist_next(param_ptr);
                    }
        }
        node_ptr = jack_slist_next(node_ptr);
    }    
}
