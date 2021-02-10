#include "handshake.h"
#include <jack/jack.h>
#include <jack/types.h>
#include <iostream>

void jack_shutdown(void *arg);

handshake::handshake(const char* clientname){

    /*set a unique name to client*/
    this->client_name=clientname;

	jack_options_t options = JackUseExactName;//(JackSessionID|JackServerName|JackNoStartServer|JackUseExactName|JackNullOption)
	jack_status_t status;//return info regarding "open client" operation

    if (status & JackNameNotUnique) {    //client name not unique, set a client name;
        client_name = jack_get_client_name(client);
        std::cerr<<"unique name "<<client_name<<"assigned to the client obj."<<std::endl;
    }

    /* open a client connection to the JACK server */
	client = jack_client_open (client_name, options, &status);

	if (client == NULL) {
        std::cerr<<"jack_client_open() failed, status = "<<status<<std::endl;
        if (status & JackServerFailed) {
            std::cerr<<"Unable to connect to JACK server"<<std::endl;
        }
        exit (1);
    }

    if (status & JackServerStarted) {
        std::cout<<"JACK server started"<<std::endl;
    }

    /*Register physical source ports*/
    fromdevice = jack_get_ports (client, NULL, NULL,JackPortIsPhysical|JackPortIsOutput);
    if (fromdevice == NULL) {
        std::cerr<<"no physical capture devices"<<std::endl;
        exit (1);
    }

    /*Register physical destination ports*/
    todevice = jack_get_ports (client, NULL, NULL, JackPortIsPhysical|JackPortIsInput);
    if (todevice == NULL) {
        std::cerr<<"no physical playback devices"<<std::endl;
        exit (1);
    }
}

handshake::~handshake(){
    free (fromdevice);
    free (todevice);
}


//########################################################################################################################
/*track operations*/
//########################################################################################################################

/* create input port for each individual track*/
jack_port_t* handshake::add_input_port(jack_port_t *_port,const char *inport_name){

    //arg 0-->, buffer_size	must be non-zero if this is not a built-in port_type. Otherwise, it is ignored.
    _port = jack_port_register(client,inport_name,JACK_DEFAULT_AUDIO_TYPE,JackPortIsInput, 0);

    if ((_port == NULL)) {
        std::cerr<<jack_port_name(_port)<<" ERROR : no more JACK ports available"<<std::endl;
        exit (1);
    }

    return _port;
}

/* create output port for each track. This ought to be called once for mono and twice for stereo*/
jack_port_t* handshake::add_output_port(jack_port_t *_port,const char *outport_name){

    _port=jack_port_register (client,outport_name,JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

    if (_port == NULL){
        std::cerr<<"Unable to register output port for {"<<jack_port_name(_port)<<"}"<<std::endl;
        exit (1);
    }

    return _port;
}

/* used for muting connections. For reconnection "add_input_port" member method is used*/
void handshake::disconnect_port(jack_port_t *_port){

    if(jack_port_disconnect(client,_port)){
        std::cerr<<"Couldnt disconnect the {"<<jack_port_name(_port)<<"} port from the main stream"<<std::endl;}
}

//########################################################################################################################
/*IO handshake operations*/
//########################################################################################################################

/*add a one side handshake between input_device and track_input_port*/
void handshake::connect_input_device(int input_device,jack_port_t *_port){


    if (jack_connect (client, fromdevice[input_device],jack_port_name(_port))){//returns full name
        std::cerr<<"cannot connect input device {"<<fromdevice[input_device]<<
        "} with input port {"<<jack_port_name(_port)<<"}"<<std::endl;
    }
}

/*add a one side handshake between track_output_port and output_device*/
void handshake::connect_output_device(int output_device,jack_port_t *_port){

    if (jack_connect (client,jack_port_name(_port), todevice[output_device])) {
        std::cerr<<"cannot connect output device {"<<todevice[output_device]<<
        "} with output port {"<<jack_port_name(_port)<<"}"<<std::endl;
    }

}

//########################################################################################################################
/*controlling operations*/
//########################################################################################################################

/* Tell the JACK server that we are ready to roll and that the client's callback will start running now. */
void handshake::activate(){
    if (jack_activate (client)) {
        std::cerr<<"cannot activate client {"<<"PUT HERE client_name"<<"}"<<std::endl;
        exit (1);
    }
}

/*terminate the client upon exit*/
void handshake::disconnect_client(){
    jack_client_close (client);
}

/* call jack_shutdown()*/
void handshake::prevent_failure(){
    void (*sd)(void *arg);
    sd=&jack_shutdown;
    jack_on_shutdown(client,&jack_shutdown,0);
}

//########################################################################################################################
/*audio stream operation*/
//########################################################################################################################

/* calls the process function whenever there is work to be done */
void handshake::callback(int(*process)(jack_nframes_t nframes, void *arg),void *arg){
    if (jack_set_process_callback (client,process,arg)){
            std::cerr<<"Callback operation failed"<<std::endl;
    }
}

//########################################################################################################################
/*control info operations*/
//########################################################################################################################

/*print out configuration info*/
void handshake::info_control(){
    std::cout<<"Sampling rate is set to: {"<<handshake::get_sample_rate()<<"}"<<std::endl;
    std::cout<<"Buffer size is set to {"<<handshake::get_buffer_size()<<"}"<<std::endl;
    std::cout<<"Current CPU load : {"<<handshake::get_cpu_load()<<"}"<<std::endl;
}

/*
const char* handshake::cpu_percent(){
    float load=jack_cpu_load(client);
    int cload=(load*100);
    std::string sload=std::to_string(cload);
    sload+="%";
    const char *prc = sload.c_str();
    return prc;
}*/

