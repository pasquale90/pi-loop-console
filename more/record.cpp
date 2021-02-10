#include <iostream>
#include <math.h>
#include <signal.h>
#include <cstring>
#include <thread>
#include <string>
#include <vector>

#include <jack/jack.h>
#include "AudioFile.h"

#define USER_PATH "C:\\Users\\alco_negro\\Desktop\\myfirstLoop.wav"

jack_port_t *output_port1, *output_port2;
jack_port_t *input_port;
jack_client_t *client;

//manual triggers, no handler implementation yet
bool trigger_loop_start=false;
bool trigger_loop_stop=false;
std::vector<float> myrec;


int monitor(jack_nframes_t nframes){
        float *in,*left,*right;
        in = (float *)jack_port_get_buffer (input_port, nframes);
        left = (float *)jack_port_get_buffer (output_port1, nframes);
        right = (float *)jack_port_get_buffer (output_port2, nframes);
        std::memcpy (left, in,
                sizeof (jack_default_audio_sample_t) * nframes);
        std::memcpy (right, in,
                sizeof (jack_default_audio_sample_t) * nframes);
                return 0;
}

//WORKS
void record(jack_nframes_t nframes){

    float *in =(float *) jack_port_get_buffer (input_port, nframes);

    for (decltype(myrec.size()) c=0;c<nframes;++c){
        myrec.push_back(*(in+c));
    }
}

//DOESN T WORK
void loop(jack_nframes_t nframes){
    static int point=0;
    std::vector<float>::iterator loop=myrec.begin()+point;

    if (point>=myrec.size()){
        point=0;
    }

    float *left = (float *)jack_port_get_buffer (output_port1, nframes);
    float *right = (float *)jack_port_get_buffer (output_port2, nframes);

    std::memcpy (left, &(*loop),
                sizeof (jack_default_audio_sample_t) * nframes);
    std::memcpy (right, &(*loop),
                sizeof (jack_default_audio_sample_t) * nframes);
    point+=nframes;
}

//WORKS!
void store(){

    AudioFile<float> audioFile;
    AudioFile<float>::AudioBuffer buffer;

    buffer.resize (2);
    // 3. Set number of samples per channel
    buffer[0].resize (myrec.size());
    buffer[1].resize(myrec.size());

    int numChannels = 2;
    int numSamplesPerChannel = myrec.size();
    float sampleRate = 44100.f;
    float frequency = 440.f;

    for (int i = 0; i < numSamplesPerChannel; i++)
    {
        for (int channel = 0; channel < numChannels; channel++)
            buffer[channel][i] = myrec.at(i);
    }

    bool ok=audioFile.setAudioBuffer(buffer);

    audioFile.save (USER_PATH, AudioFileFormat::Wave);
}


//function pointers. No need in that. Just for practice.
void (*rfp)(jack_nframes_t nframes)=&record;
void (*pfp)(jack_nframes_t nframes)=&loop;
void (*sfp)(void)=&store;


int
processes (jack_nframes_t nframes, void* arg){

         monitor(nframes);


        if (trigger_loop_start){
          rfp(nframes);
        }

        if (trigger_loop_stop && !trigger_loop_start){
            pfp(nframes);
        }
}

void
jack_shutdown (void *arg)
{
	exit (1);
}

int
main (int argc, char *argv[])
{
	const char **ports;
	const char *client_name;
	const char *server_name = NULL;
	jack_options_t options = JackNullOption;
	jack_status_t status;


    /* DEFINE CLIENT NAME = SAMPLE CODE FROM JACK_AUDIO simple_client.c example */
	if (argc >= 2) {
		client_name = argv[1];
		if (argc >= 3) {	/* server name specified? */
			server_name = argv[2];
			int my_option = JackNullOption | JackServerName;
			options = (jack_options_t)my_option;
		}
	} else {
		client_name = strrchr(argv[0], '/');
		if (client_name == 0) {
			client_name = argv[0];
		} else {
			client_name++;
		}
	}




	client = jack_client_open (client_name, options, &status);


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

    jack_set_process_callback(client,processes,0);

	jack_on_shutdown (client, jack_shutdown, 0);


	/* create two ports */
    input_port = jack_port_register (client, "input",
                                         JACK_DEFAULT_AUDIO_TYPE,
                                         JackPortIsInput, 0);

	output_port1 = jack_port_register (client, "output1",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsOutput, 0);

	output_port2 = jack_port_register (client, "output2",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsOutput, 0);


	if ((input_port == NULL)||(output_port1 == NULL) || (output_port2 == NULL)) {
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

	if (jack_connect (client, jack_port_name (output_port1), ports[0])) {
		fprintf (stderr, "cannot connect output ports\n");
	}

	if (jack_connect (client, jack_port_name (output_port2), ports[1])) {
		fprintf (stderr, "cannot connect output ports\n");
	}

	jack_free (ports);


    ///*
	std::cout<<"Give input"<<std::endl;
    std::string input;
    while(std::cin>>input){
        if (input=="R"){
            trigger_loop_start=true;
        }
        if (input=="S"){

            std::cout<<"SAMPLE HAS "<<myrec.size()/44100.f<<" LENGTH"<<std::endl;
            trigger_loop_start=false;
            trigger_loop_stop=true;
            store();
        }
    }//*/

	jack_client_close (client);
	exit (0);
}
