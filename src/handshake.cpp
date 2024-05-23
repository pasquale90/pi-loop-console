#include "handshake.h"

int streamAudio (jack_nframes_t nframes, void *arg){ //, float *in,void (*threading)(float *sig)
    return static_cast<Handshake*>(arg)->process(nframes);
}

/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client does nothing more than copy data from its input
 * port to its output port. It will exit when stopped by 
 * the user (e.g. using Ctrl-C on a unix-ish operating system)
 */
int Handshake::process (jack_nframes_t nframes)
{

// input and output must be handled by Monitor and Mixer
// After that, how to pass data to the Looper entity?

	jack_default_audio_sample_t *in, *leftout,*rightout;
	
	in = (float*)jack_port_get_buffer (input_port_mic, nframes);
	leftout = (float*)jack_port_get_buffer (output_port_left, nframes);
    rightout = (float*)jack_port_get_buffer (output_port_right, nframes);

	memcpy (leftout, in,sizeof (jack_default_audio_sample_t) * nframes);
    memcpy (rightout, in,sizeof (jack_default_audio_sample_t) * nframes);

	return 0;      
}

Handshake::Handshake(){
    server_name = NULL;
    // clientThread = std::thread(&Handshake::setup,this);
    // frameToggle.store(false);
    is_running.store(true);
    is_firsTime.store(true);
    reinitialization.store(true);

    // ClientStatus client_status();

    std::cout<<"\n\n######### HS Constructor###############"<<std::endl;
    std::cout<<"is_running :"<<is_running.load()<<"\nreinitialization :"<<reinitialization.load()<<"\nis_firsTime :"<<is_firsTime.load()<<std::endl;
    std::cout<<"\n\n######### (end)HS Constructor###############"<<std::endl;
    // start_server();
}

// Handshake::~Handshake(){
//     std::cout<<"--------------------------------> ~Handshake is called!!!!"<<std::endl;
//     free (from_device);
//     free (to_device);
//     disconnect();
//     // audio_server.stop();
// }

// void Handshake::start_server(){
//     std::thread serverThread(&AudioServer::start, &audio_server);
//     serverThread.join();
// }

void Handshake::connect(bool verbose){

    client_status.link_client = link_client();
    client_status.set_process_callback = set_process_callback();

    prevent_failure();
    
// @TODO conditions require further work
    //initialize ports
    // if (cfg.device_settings.micIn)
        input_port_mic = register_input_port("microphone");
    // if (cfg.device_settings.instIn)
        // input_port_inst = register_input_port("instrunment");
    // if (cfg.device_settings.stereoOut)
        output_port_left = register_output_port("left_speaker");
        output_port_right = register_output_port("right_speaker");
    
// change server operations
    client_status.jack_set_buffer_size = jack_set_buffer_size (client,cfg.audio_settings.buffer_size);

// @TODO latency_ms

    client_status.activate = activate();
    client_status.register_devices = register_devices();

// @TODO check from and to device length of data structures
    // int i=0;
    // std::cout<<"FROM DEVICE "<<std::endl;
    // while(from_device[i]){
    //     std::cout<<from_device[i]<<" ";
    //     ++i;
    // }std::cout<<std::endl;
    // i=0;
    // std::cout<<"TO DEVICE "<<std::endl;
    // while(to_device[i]){
    //     std::cout<<to_device[i]<<" ";
    //     ++i;
    // }std::cout<<std::endl;
// @TODO check from and to device length of data structures <-same as above
    connect_input_device(0,"microphone");
    // connect_input_device(1,"instrunment");
    connect_output_device(0,"left_speaker");
    connect_output_device(1,"right_speaker");

    client_status.client_connected=true;
    
    this->verbose=verbose;
    check_status(); // default value : verbose = false;
}

void Handshake::disconnect(){
    std::cout<<"Disconnecting client"<<std::endl;

    free (from_device);
    free (to_device);

    jack_client_close (client);
}

void Handshake::reInitialize(){
    reinitialization.store(true);
}

void Handshake::stop_running(){
    is_running.store(false);
}

// void Handshake::setup(){
//     // int i=0;
//     while(is_running){
//         // if (i%10)
//             std::cout<<"tick tock"<<std::endl;
//         // ++i;
//         bool reinit = reinitialization.load();
//         if (reinit == true){          
//             reinitialization.store(false);
//             std::cout<<"\n\nHandshake :: is_running :"<<is_running.load()<<"\nreinitialization :"<<reinitialization.load()<<"\nis_firsTime :"<<is_firsTime.load()<<std::endl;          
//             bool ft =is_firsTime.load(); 
//             if (ft == true){
//                 is_firsTime.store(false);
//             }else{
//                 std::cout<<"\n\nHandshake ::Disconnecting handshake"<<std::endl;
//                 disconnect();
//                 // sleep(2);
// // std::this_thread::sleep_for (std::chrono::seconds(2));
//                 std::cout<<"\n\n\nHandshake ::Disconnection of client "<<client_name<<" is set!!!\n\n\n"<<std::endl;
//             }
//             std::cout<<"\n\nHandshake ::Connecting handshake"<<std::endl;
//             connect();
// // std::this_thread::sleep_for (std::chrono::seconds(3));
//             // sleep(3);
//             std::cout<<"\n\n\nHandshake :: Connection of client "<<client_name<<" is set!!!\n\n\n"<<std::endl;
//         }   
//     }
//     std::cout<<"-----------------------------------------------------------------------------Handshake --------------------------------------------------------->Stopped running"<<std::endl;
// }

void Handshake::setup(){
    
    while(is_running){
        
        bool reinit = reinitialization.load();
        if (reinit == true){          
            reinitialization.store(false);
            bool ft =is_firsTime.load(); 
            if (ft == true){
                is_firsTime.store(false);
            }else{
                disconnect();
            }
            connect();
        }   
    }
}

char* Handshake::_reset_client_name(){
    // std::cout<<"cfg.currSession_name "<<cfg.currSession_name<<std::endl;
    std::string clientName = "client_"+ cfg.currSession_name;
    // std::cout<<"clientName "<<clientName<<std::endl;
    return &clientName[0]; //.data();
    // return clientName.data();
}

inline const char* _concat_chars(const char* ch1, const char* ch2){
    // int s1 = sizeof(ch1)/sizeof(ch1[0]);
    // int s2 = sizeof(ch2)/sizeof(ch2[0]);
    char *s = new char[20];
    strcpy(s,ch1);
    strcat(s,ch2);
    return s;
}

int Handshake::link_client(){
    jack_options_t options = JackNullOption;
    jack_status_t status;
    
    /* open a client connection to the JACK server */
    client_name = &cfg.currSession_name[0];
    client = jack_client_open (const_cast<char*>(client_name), options, &status, server_name);
    
    if (client == NULL) {
        fprintf (stderr, "jack_client_open() failed, "
            "status = 0x%2.0x\n", status);
        if (status & JackServerFailed) {
            fprintf (stderr, "Unable to connect to JACK server\n");
        }
        return status;
    }
    if (status & JackServerStarted) {
        fprintf (stderr, "JACK server started\n");
    }
    if (status & JackNameNotUnique) {
        client_name = jack_get_client_name(client);
        fprintf (stderr, "unique name `%s' assigned\n", client_name);
    }
    std::cout<<"Handshake::link_client::status ----> "<<status<<std::endl;
    return status;
}

int Handshake::set_process_callback(){
     // tell the JACK server to call `process()' whenever
    // there is work to be done.
   return jack_set_process_callback (client, streamAudio, this);

}

bool Handshake::register_devices(){
    
    from_device = jack_get_ports (client, NULL, NULL,
                JackPortIsPhysical|JackPortIsOutput);
    if (from_device == NULL) {
        fprintf(stderr, "no physical capture ports\n");
        return false;
    }

    to_device = jack_get_ports (client, NULL, NULL,
                JackPortIsPhysical|JackPortIsInput);
    if (to_device == NULL) {
        fprintf(stderr, "no physical playback ports\n");
        return false;
    }
    return true;
}

void Handshake::connect_input_device(int input_device,const char* name){
    
// put in private member function -> get/form port name
    // int sz = sizeof(client_name)/sizeof(client_name[0]) + sizeof(name)/sizeof(name[0]) + 2;
    char ffname[20];
    strcpy(ffname,client_name);
    strcat(ffname,":");
    strcat(ffname,name);
    const char *fname = ffname;

    // char* fname = _concat_chars(client_name,":");
    // fname = _concat_chars(fname,name);

    if (jack_connect (client, from_device[input_device],const_cast<char*>(fname))){//returns full name
        std::cerr<<"cannot connect input device {"<<from_device[input_device]<<
        "} with input port {"<<fname<<"}"<<std::endl;
    } else{
        std::cout<<"\t\tinput device {"<<from_device[input_device]<<
        "} with input port {"<<fname<<"} are now connected!"<<std::endl;
    }
}
void Handshake::connect_output_device(int output_device, const char* name){

// put in private member function -> get/form port name
    // int sz = sizeof(client_name)/sizeof(client_name[0]) + sizeof(name)/sizeof(name[0]) + 2;
    char ffname[20];
    strcpy(ffname,client_name);
    strcat(ffname,":");
    strcat(ffname,name);
    const char *fname = ffname;

    if (jack_connect (client,fname, to_device[output_device])) {
        std::cerr<<"cannot connect output device {"<<to_device[output_device]<<
        "} with output port {"<<fname<<"}"<<std::endl;
    }else{
        std::cout<<"\t\toutput device {"<<to_device[output_device]<<
        "} with output port {"<<fname<<"} are now connected!"<<std::endl;
    }
}

void Handshake::mute_microphone()
{   
    if(jack_port_disconnect(client,input_port_mic)){
        std::cerr<<"Couldnt disconnect the {"<<jack_port_name(input_port_mic)<<"} port from the main stream"<<std::endl;}
}

void Handshake::unmute_microphone()
{   
    connect_input_device(0,"microphone");
}

void Handshake::mute_instrument()
{   
    if(jack_port_disconnect(client,input_port_inst)){
        std::cerr<<"Couldnt disconnect the {"<<jack_port_name(input_port_inst)<<"} port from the main stream"<<std::endl;}
}

void Handshake::unmute_instrument()
{   
    connect_input_device(1,"instrunment");
}


void Handshake::info_control (){
    
    std::cout<<"------------------------- Check status of client "<<client_name<<" ------------------------- "<<std::endl;
    bool isOk = true;
    
    if (!client_status.samplingRate){
        isOk=false;
        std::cout<<"Handshake::info_control : cfg mismatch -> Sampling rate is set to: {"<<get_sample_rate()<<"!="<<cfg.audio_settings.sample_rate<<"}"<<std::endl;
    }else std::cout<<"Handshake::info_control : Sampling rate : OK "<<std::endl;

    if (!client_status.bufferSize){
        isOk=false;
        std::cout<<"Handshake::info_control : cfg mismatch -> Buffer size is set to {"<<get_buffer_size()<<"!="<<client_status.bufferSize<<"}"<<std::endl;
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

    if (client_status.jack_set_buffer_size != 0){
        isOk=false;
        std::cout<<"Handshake::info_control : ERROR -> Handshake::jack_set_buffer_size failed with error: {"<<client_status.jack_set_buffer_size<<"}"<<std::endl;
    }else std::cout<<"Handshake::info_control : jack_set_buffer_size : OK "<<std::endl;

    if (!client_status.activate){
        isOk=false;
        std::cout<<"Handshake::info_control : ERROR -> client is not activated"<<std::endl;
    }else std::cout<<"Handshake::info_control : activate : OK "<<std::endl;

    if (!client_status.register_devices){
        isOk=false;
        std::cout<<"Handshake::info_control : ERROR -> devices are not registered"<<std::endl;
    }else std::cout<<"Handshake::info_control : register_devices : OK "<<std::endl;

    if (!client_status.client_connected){
        isOk=false;
        std::cout<<"Handshake::info_control : ERROR -> client NOT connected"<<std::endl;
    }else std::cout<<"Handshake::info_control : client_connected : OK "<<std::endl;
    
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

void Handshake::check_status(){
    client_status.samplingRate = ((uint32_t)cfg.audio_settings.sample_rate) == get_sample_rate();
    client_status.bufferSize = ((uint32_t)cfg.audio_settings.buffer_size) == get_buffer_size();
    client_status.isRealTime = realTime_enabled();
    client_status.cpuload = get_cpu_load();

    if (verbose)
        info_control();
}

uint32_t Handshake::get_sample_rate (){
    return (uint32_t)jack_get_sample_rate (client);
}

uint32_t Handshake::get_buffer_size (){
    return (uint32_t)jack_get_buffer_size (client);
}

float Handshake::get_cpu_load (){
    return jack_cpu_load(client);
}

bool Handshake::realTime_enabled(){
    return jack_is_realtime(client);
}

void Handshake::prevent_failure(){
    std::cout<<"Preventing failure"<<std::endl;
    // tell the JACK server to call `jack_shutdown()' if
    // it ever shuts down, either entirely, or if it
    // just decides to stop calling us.
    jack_on_shutdown (client, &jack_shutdown, this);
}
    
void Handshake::jack_shutdown(void *arg){
    std::cout<<"on shutdown"<<std::endl;
	exit (1);
    // this.disconnect();
}

jack_port_t* Handshake::register_input_port(const char* name){
    /* create two ports */

    jack_port_t* input_port = jack_port_register (client, name,
                    JACK_DEFAULT_AUDIO_TYPE,
                    JackPortIsInput, 0);

    if (input_port == NULL) {
        fprintf(stderr, "no more JACK ports available\n");
        return nullptr;
    }
    return input_port;

}

jack_port_t* Handshake::register_output_port(const char* name){
    jack_port_t *output_port = jack_port_register (client, name,
                    JACK_DEFAULT_AUDIO_TYPE,
                    JackPortIsOutput, 0);

    if (output_port == NULL) {
        fprintf(stderr, "no more JACK ports available\n");
        return nullptr;
    }
    return output_port;
}

bool Handshake::activate(){

    /* Tell the JACK server that we are ready to roll.  Our
    * process() callback will start running now. */

    if (jack_activate (client)) {
        fprintf (stderr, "cannot activate client");
        return false;
    }
    return true;
}