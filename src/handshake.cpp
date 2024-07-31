#include "handshake.h"
// #define VERBOSE

static const float cpuLoad_thress = 0.8;

static const char* input_port_names[MAX_INPUTS] = {"microphone", "instrument"};
#if STEREO_OUT
        static const char* output_port_names[F_NUM_OUTPUTS] = {"left_speaker", "right_speaker"};
#else
        static const char* output_port_names[F_NUM_OUTPUTS] = {"mono_speaker"};
#endif

int callback_f (jack_nframes_t,void*);

Handshake::Handshake(){
    server_name = NULL;
}

void Handshake::link_client(char* name){
    jack_options_t options = JackNullOption;
    jack_status_t status;

    /* open a client connection to the JACK server */
    // client_name = &cfg.currSession_name[0];
    client_name = name;
    client = jack_client_open (client_name, options, &status, server_name);
    
    if (client == NULL) {
        fprintf (stderr, "jack_client_open() failed, "
            "status = 0x%2.0x\n", status);
        if (status & JackServerFailed) {
            fprintf (stderr, "Unable to connect to JACK server\n");
        }
        client_status.link_client = status;
    }
    if (status & JackServerStarted) {
        fprintf (stderr, "JACK server started\n");
    }
    if (status & JackNameNotUnique) {
        client_name = jack_get_client_name(client);
        fprintf (stderr, "unique name `%s' assigned\n", client_name);
    }
    client_status.link_client = status;
}

void Handshake::set_process_callback(){
    // tell the JACK server to call `process()' whenever
    // there is work to be done.
   client_status.set_process_callback = jack_set_process_callback (client, callback_f, this);
}

void Handshake::prevent_failure(){
    // tell the JACK server to call `jack_shutdown()' if
    // it ever shuts down, either entirely, or if it
    // just decides to stop calling us.
    jack_on_shutdown (client, &_jack_shutdown, this);
}

void Handshake::register_input_port(int idx){
    /* create two ports */
    input_ports[idx] = jack_port_register (client, input_port_names[idx],
                    JACK_DEFAULT_AUDIO_TYPE,
                    JackPortIsInput, 0);
    if (input_ports[idx] == NULL) {
        fprintf(stderr, "no more JACK ports available\n");
        return;
    }
}

void Handshake::register_output_port(int idx){
    output_ports[idx] = jack_port_register (client,  output_port_names[idx],
                    JACK_DEFAULT_AUDIO_TYPE,
                    JackPortIsOutput, 0);
    if (output_ports[idx] == NULL) {
        fprintf(stderr, "no more JACK ports available\n");
        return;
    }
}

void Handshake::activate(){
    /* Tell the JACK server that we are ready to roll.  Our
    * process() callback will start running now. */
    if (jack_activate (client)) {
        fprintf (stderr, "cannot activate client");
        client_status.activate = false;
        return;
    }
    client_status.activate =  true;
}

void Handshake::register_devices(){
    from_device = jack_get_ports (client, NULL, NULL,
                JackPortIsPhysical|JackPortIsOutput);
    if (from_device == NULL) {
        fprintf(stderr, "no physical capture ports\n");
        client_status.register_devices = false;
        return;
    }
    to_device = jack_get_ports (client, NULL, NULL,
                JackPortIsPhysical|JackPortIsInput);
    if (to_device == NULL) {
        fprintf(stderr, "no physical playback ports\n");
        client_status.register_devices = false;
        return;
    }
    client_status.register_devices = true;
}

void Handshake::set_buffer_size(){
    client_status.set_buffer_size =  jack_set_buffer_size (client,BUFFER_SIZE);
}

void Handshake::check_status(){
    client_status.samplingRate = ((uint32_t)SAMPLE_RATE) == _get_sample_rate();
    client_status.bufferSize = ((uint32_t)BUFFER_SIZE) == _get_buffer_size();
    client_status.isRealTime = _realTime_enabled();
    client_status.cpuload = _get_cpu_load();
    _info_control();
}


void Handshake::disconnect_client(){
    std::cout<<"Disconnecting client"<<std::endl;
    free (from_device);
    free (to_device);
    jack_client_close (client);
}


void Handshake::connect_input_device(int idx){
// put in private member function -> get/form port name
    // int sz = sizeof(client_name)/sizeof(client_name[0]) + sizeof(name)/sizeof(name[0]) + 2;
    char ffname[20];
    strcpy(ffname,client_name);
    strcat(ffname,":");
    strcat(ffname,input_port_names[idx]);
    const char *fname = ffname;
    if (jack_connect (client, from_device[idx],const_cast<char*>(fname))){//returns full name
        std::cerr<<"cannot connect input device {"<<from_device[idx]<<
        "} with input port {"<<fname<<"}"<<std::endl;
    } else{
        std::cout<<"\t\tinput device {"<<from_device[idx]<<
        "} with input port {"<<fname<<"} are now connected!"<<std::endl;
    }
}

void Handshake::connect_output_device(int idx){
// put in private member function -> get/form port name
    // int sz = sizeof(client_name)/sizeof(client_name[0]) + sizeof(name)/sizeof(name[0]) + 2;
    char ffname[20];
    strcpy(ffname,client_name);
    strcat(ffname,":");
    strcat(ffname,output_port_names[idx]);
    const char *fname = ffname;
    if (jack_connect (client,fname, to_device[idx])) {
        std::cerr<<"cannot connect output device {"<<to_device[idx]<<
        "} with output port {"<<fname<<"}"<<std::endl;
    }else{
        std::cout<<"\t\toutput device {"<<to_device[idx]<<
        "} with output port {"<<fname<<"} are now connected!"<<std::endl;
    }
}

void Handshake::disconnect_input_device(int idx)
{   
    if (jack_port_connected(input_ports[idx])){
        if(jack_port_disconnect(client,input_ports[idx])){
            std::cerr<<"Couldnt disconnect the {"<<input_port_names[idx]<<"} port from the main stream"<<std::endl;
        }
    }
}

void Handshake::disconnect_output_device(int idx)
{       
    if (jack_port_connected(output_ports[idx])){
        if(jack_port_disconnect(client,output_ports[idx])){
            std::cerr<<"Couldnt disconnect the {"<<output_port_names[idx]<<"} port from the main stream"<<std::endl;
        }
    }
}


float* Handshake::get_input_buffer(int idx){
    return (float*)jack_port_get_buffer (input_ports[idx], BUFFER_SIZE);
}

float* Handshake::get_output_buffer(int idx){
    return (float*)jack_port_get_buffer (output_ports[idx], BUFFER_SIZE);
}

// char* Handshake::_reset_client_name(){
//     std::string clientName = "client_"+ cfg.currSession_name;
//     return &clientName[0]; //.data();
//     // return clientName.data();
// }

// inline const char* _concat_chars(const char* ch1, const char* ch2){
//     char s[20];
//     strcpy(s,ch1);
//     strcat(s,ch2);
//     return s;
// }

void Handshake::_jack_shutdown(void *arg){
    (void)arg; //!supress_warning unsused parameter
	exit (1);
    // this.disconnect();
}

uint32_t Handshake::_get_sample_rate (){
    return (uint32_t)jack_get_sample_rate (client);
}

uint32_t Handshake::_get_buffer_size (){
    return (uint32_t)jack_get_buffer_size (client);
}

float Handshake::_get_cpu_load (){
    return jack_cpu_load(client);
}

bool Handshake::_realTime_enabled(){
    return jack_is_realtime(client);
}

void Handshake::_info_control (){
    std::cout<<"------------------------- Check status of client "<<client_name<<" ------------------------- "<<std::endl;
    bool isOk = true;
    
    if (!client_status.samplingRate){
        isOk=false;
        std::cout<<"Handshake::info_control : cfg mismatch -> Sampling rate is set to: {"<<_get_sample_rate()<<"!="<<SAMPLE_RATE<<"}"<<std::endl;
    }else std::cout<<"Handshake::info_control : Sampling rate : OK "<<std::endl;

    if (!client_status.bufferSize){
        isOk=false;
        std::cout<<"Handshake::info_control : cfg mismatch -> Buffer size is set to {"<<_get_buffer_size()<<"!="<<client_status.bufferSize<<"}"<<std::endl;
    }else std::cout<<"Handshake::info_control : Buffer size : OK "<<std::endl;

    if (!client_status.isRealTime){
        isOk=false;
        std::cout<<"Handshake::info_control : WARNING -> Real time enabled: {"<<client_status.isRealTime<<"}"<<std::endl;
    }else std::cout<<"Handshake::info_control : Real Time support : OK "<<std::endl;

    if ( client_status.cpuload >= cpuLoad_thress ){
        isOk=false;
        std::cout<<"Handshake::info_control : WARNING -> Current CPU load : {"<<client_status.cpuload<<"}"<<std::endl;
    }else std::cout<<"Handshake::info_control : CPU load : OK (CPU load = "<<client_status.cpuload<<")"<<std::endl;

    if (client_status.link_client != 0){
        isOk=false;
        std::cout<<"Handshake::info_control : ERROR ->  : Handshake::link_client failed with error {"<<client_status.link_client<<"}"<<std::endl;
    }else std::cout<<"Handshake::info_control : link_client : OK "<<std::endl;

    if (client_status.set_process_callback != 0){
        isOk=false;
        std::cout<<"Handshake::info_control : ERROR -> Handshake::set_process_callback failed with error : {"<<client_status.set_process_callback<<"}"<<std::endl;
    }else std::cout<<"Handshake::info_control : set_process_callback : OK "<<std::endl;

    if (client_status.set_buffer_size != 0){
        isOk=false;
        std::cout<<"Handshake::info_control : ERROR -> Handshake::set_buffer_size failed with error: {"<<client_status.set_buffer_size<<"}"<<std::endl;
    }else std::cout<<"Handshake::info_control : set_buffer_size : OK "<<std::endl;

    if (!client_status.activate){
        isOk=false;
        std::cout<<"Handshake::info_control : ERROR -> client is not activated"<<std::endl;
    }else std::cout<<"Handshake::info_control : activate : OK "<<std::endl;

    if (!client_status.register_devices){
        isOk=false;
        std::cout<<"Handshake::info_control : ERROR -> devices are not registered"<<std::endl;
    }else std::cout<<"Handshake::info_control : register_devices : OK "<<std::endl;

    // if (!client_status.client_connected){
    //     isOk=false;
    //     std::cout<<"Handshake::info_control : ERROR -> client NOT connected"<<std::endl;
    // }else std::cout<<"Handshake::info_control : client_connected : OK "<<std::endl;
    
// @TODO later

    // if (status.latency_ms > 15){
    //     isOk=false;
    //     std::cout<<"Handshake::info_control : WARNING -> Current latency exceeds 15ms : {"<<status.latency_ms<<"}(ms)"<<std::endl;
    // }else std::cout<<"Handshake::info_control : Latency : OK "<<std::endl;

    if ( isOk ){
        std::cout<<"Handshake::info_control : OK"<<std::endl;
    }else std::cout<<"Handshake::info_control : check \'\'Handshake::info_control\'\' above.."<<std::endl;

    std::cout<<"------------------------------------------------------------------------------------ "<<std::endl;
}