#include "handshake.h"

int streamAudio (jack_nframes_t nframes, void *arg){ //, float *in,void (*threading)(float *sig)
    return static_cast<AudioStream*>(arg)->process(nframes);
}

/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client does nothing more than copy data from its input
 * port to its output port. It will exit when stopped by 
 * the user (e.g. using Ctrl-C on a unix-ish operating system)
 */
int AudioStream::process (jack_nframes_t nframes)
{
	jack_default_audio_sample_t *in, *out;
	
	in = (float*)jack_port_get_buffer (input_port, nframes);
	out = (float*)jack_port_get_buffer (output_port, nframes);
	memcpy (out, in,
		sizeof (jack_default_audio_sample_t) * nframes);

	return 0;      
}

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void
jack_shutdown (void *arg)
{
	exit (1);
}

void AudioStream::run_server(int argc, char *argv[])
    {
        const char **ports;
        const char *client_name = "simple";
        const char *server_name = NULL;
        jack_options_t options = JackNullOption;
        jack_status_t status;

        // const char *server_name = "NewJackServer";
        // jack_options_t options = JackServerName;
        
        
        
        /* open a client connection to the JACK server */

        client = jack_client_open (client_name, options, &status, server_name);
        if (client == NULL) {
            fprintf (stderr, "jack_client_open() failed, "
                "status = 0x%2.0x\n", status);
            if (status & JackServerFailed) {
                fprintf (stderr, "Unable to connect to JACK server\n");
            }
            exit (1);
        }
        if (status & JackServerStarted) {
            fprintf (stderr, "JACK server started\n");
        }
        if (status & JackNameNotUnique) {
            client_name = jack_get_client_name(client);
            fprintf (stderr, "unique name `%s' assigned\n", client_name);
        }

        /* tell the JACK server to call `process()' whenever
        there is work to be done.
        */

        jack_set_process_callback (client, streamAudio, this);

        /* tell the JACK server to call `jack_shutdown()' if
        it ever shuts down, either entirely, or if it
        just decides to stop calling us.
        */

        jack_on_shutdown (client, jack_shutdown, 0);

        /* display the current sample rate. 
        */

    //Check if realtime	
        // if (jack_is_realtime(client)){
        // 	printf("Jack is realtime -----------------> ");
        // }
        // else printf("----------------->  Jack is NOT realtime ");
        

        printf ("engine sample rate: %" PRIu32 "\n",
            jack_get_sample_rate (client));

        /* create two ports */

        input_port = jack_port_register (client, "input",
                        JACK_DEFAULT_AUDIO_TYPE,
                        JackPortIsInput, 0);
        output_port = jack_port_register (client, "output",
                        JACK_DEFAULT_AUDIO_TYPE,
                        JackPortIsOutput, 0);

        if ((input_port == NULL) || (output_port == NULL)) {
            fprintf(stderr, "no more JACK ports available\n");
            exit (1);
        }

        /* Tell the JACK server that we are ready to roll.  Our
        * process() callback will start running now. */

        if (jack_activate (client)) {
            fprintf (stderr, "cannot activate client");
            exit (1);
        }

        /* Connect the ports.  You can't do this before the client is
        * activated, because we can't make connections to clients
        * that aren't running.  Note the confusing (but necessary)
        * orientation of the driver backend ports: playback ports are
        * "input" to the backend, and capture ports are "output" from
        * it.
        */

        ports = jack_get_ports (client, NULL, NULL,
                    JackPortIsPhysical|JackPortIsOutput);
        if (ports == NULL) {
            fprintf(stderr, "no physical capture ports\n");
            exit (1);
        }

        if (jack_connect (client, ports[0], jack_port_name (input_port))) {
            fprintf (stderr, "cannot connect input ports\n");
        }

        free (ports);
        
        ports = jack_get_ports (client, NULL, NULL,
                    JackPortIsPhysical|JackPortIsInput);
        if (ports == NULL) {
            fprintf(stderr, "no physical playback ports\n");
            exit (1);
        }

        if (jack_connect (client, jack_port_name (output_port), ports[0])) {
            fprintf (stderr, "cannot connect output ports\n");
        }

        free (ports);

        /* keep running until stopped by the user */

        sleep (-1);

        /* this is never reached but if the program
        had some other way to exit besides being killed,
        they would be important to call.
        */

        jack_client_close (client);
        exit (0);
    }


/*
int streamAudio (jack_nframes_t nframes, void *arg){ //, float *in,void (*threading)(float *sig)
    return static_cast<AudioStream*>(arg)->streamBuffer(nframes);
}

AudioStream::AudioStream(const char* clientName, const char* serverName = NULL){
    
    server_name=serverName;
    client_name=clientName;  

    // start_server();
    connect_client();
    
    AudioRouting();
}

void AudioStream::connect_client(){
    jack_options_t options = JackNoStartServer;//(JackSessionID|JackServerName|JackNoStartServer|JackUseExactName|JackNullOption)
	jack_status_t status;

std::cout<<"Debug 1"<<std::endl;

    // open a client connection to the JACK server
	client = jack_client_open (client_name, options, &status,server_name);
    if (status & JackNameNotUnique) {    //client name not unique, set a client name;
        client_name = jack_get_client_name(client);
        std::cerr<<"\t>>unique name "<<client_name<<" assigned to the client obj."<<std::endl;
    }
    
std::cout<<"Debug 2"<<std::endl;

	if (client == NULL) {
        std::cerr<<"\t>>jack_client_open() failed, status = "<<status<<std::endl;
        if (status & JackServerFailed) {
            std::cerr<<"\t>>Unable to connect to JACK server"<<std::endl;
        }
        exit (1);
    }
    if (status & JackServerStarted) {
        std::cout<<"\t>>JACK server started"<<std::endl;
    }

std::cout<<"Debug 3"<<std::endl;


}
void AudioStream::AudioRouting(){
    
    //callback
    if (jack_set_process_callback (client,streamAudio,this)){ //arg
            std::cerr<<"\t>>Callback operation failed"<<std::endl;
    }

std::cout<<"Debug 4"<<std::endl;

    //prevent failure
    jack_on_shutdown(client,&jack_shutdown,0);

std::cout<<"Debug 5"<<std::endl;

    //register physical ports
    input_port1 = jack_port_register(client,"inputPort",JACK_DEFAULT_AUDIO_TYPE,JackPortIsInput, 0);
std::cout<<"Debug 6"<<std::endl;

    output_port_left=jack_port_register (client,"leftPort",JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

std::cout<<"Debug 7"<<std::endl;

    output_port_right=jack_port_register (client,"rightPort",JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

std::cout<<"Debug 8 "<<(input_port1 == NULL)<<std::endl;

    if (input_port1 == NULL) {
        std::cout<<jack_port_name(input_port1)<<"\t>> ERROR : no more JACK ports available"<<std::endl;
        exit (1);}

std::cout<<"Debug 9"<<std::endl;

    if (output_port_left == NULL){
        std::cerr<<"\t>>Unable to register output port for {"<<jack_port_name(output_port_left)<<"}"<<std::endl;
        exit (1);}

std::cout<<"Debug 10"<<std::endl;

    if (output_port_right == NULL){
        std::cerr<<"\t>>Unable to register output port for {"<<jack_port_name(output_port_right)<<"}"<<std::endl;
        exit (1);}
    
std::cout<<"Debug 11"<<std::endl;

    //activate client
    if (jack_activate (client)) {
        std::cerr<<"\t>>cannot activate client {"<<client_name<<"}"<<std::endl;
        exit (1);
    }

    // Getting acces to physical source ports
    fromdevice = jack_get_ports (client, NULL, NULL,JackPortIsPhysical|JackPortIsOutput);
    if (fromdevice == NULL) {
        std::cerr<<"\t>>no physical capture devices"<<std::endl;
        exit (1);
    }
// register all input device channels
    // const char **temp_device=fromdevice;
    // std::cout<<"Device list Input>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n\n\n\n"<<std::endl;
    // int devCount=0;
    // while(*temp_device!=NULL){
    //     std::cout<<devCount<<":"<<*temp_device<<" ";
    //     *temp_device++;
    //     devCount++;
    // }std::cout<<"\n\n\n\n\n"<<std::endl;
    // // (K6) output prints ...
    // //Device list Input>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // //system:capture_1 1:system:capture_2 2:system:capture_3 3:system:capture_4 4:system:capture_5 5:system:capture_6 

    // Getting acces to destination ports
    todevice = jack_get_ports (client, NULL, NULL, JackPortIsPhysical|JackPortIsInput);
    if (todevice == NULL) {
        std::cerr<<"\t>>no physical playback devices"<<std::endl;
        exit (1);
    }

    //connect all
    if (input_port1!=NULL){
        if (jack_connect (client, fromdevice[0], jack_port_name(input_port1))){//returns full name
           std::cerr<<"\t>>cannot connect input device {"<<fromdevice[0]<<"} with input port {"<<jack_port_name(input_port1)<<"}"<<std::endl;
    }
    }
    if (output_port_left!=NULL && output_port_right!=NULL){
        if (jack_connect (client, jack_port_name(output_port_left), todevice[0])){//returns full name
            std::cerr<<"\t>>cannot connect left plysical output port {"<<todevice[0]<<"} with input port {"<<jack_port_name(output_port_left)<<"}"<<std::endl;}
        if (jack_connect (client, jack_port_name(output_port_right), todevice[1])){//returns full name
           std::cerr<<"\t>>cannot connect right physical output port {"<<todevice[1]<<"} with input port {"<<jack_port_name(output_port_right)<<"}"<<std::endl;}
    }

    free (fromdevice);
    free (todevice);
}


void AudioStream::closeStream(){
    jack_client_close (client);
}

int AudioStream::streamBuffer(jack_nframes_t nframes){

    //out receive values from 
    float *left,*right;

    in = (float*)jack_port_get_buffer (input_port1, nframes);
    
//TODO get that signal from EACH DEVICE------> http://www.vttoth.com/CMS/index.php/technical-notes/68

    left = (float*)jack_port_get_buffer (output_port_left, nframes);
    right= (float*)jack_port_get_buffer(output_port_right, nframes);

    // audioBufferCallback(left,right);

    // float *toneL = float[nframes];
    // float *toneR = float[nframes];
    // here apply the mixing. Note that left and right hold the sine signal returned by audioBufferCallback, so the mix should operate in place.
    std::memcpy (left, in, sizeof (float) *nframes);
    std::memcpy (right, in, sizeof (float) *nframes);

    return 0;
}

void AudioStream::mix(){
    // In case that more than 1 input sources are connected, access the number of input sources (in particular, the #input_sources that are contributing info to the aggregated *in buffer)
}

void AudioStream::jack_shutdown (void *arg)
{
    std::cout<<"AudioStream::jack_shutdown : shutting down server"<<std::endl;
	exit (1);
}
*/