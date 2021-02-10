#ifndef BEEPER_H_INCLUDED
#define BEEPER_H_INCLUDED

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (200)
typedef struct
{
	float sine[TABLE_SIZE];
	int left_phase;
	int right_phase;
}
paTestData;

static void signal_handler(int sig)
{
	jack_client_close(client);
	fprintf(stderr, "signal received, exiting ...\n");
	exit(0);
}


int beeper (jack_nframes_t nframes, void *arg)
{
    //std::cout<<std::endl<<"Nframes= "<<nframes<<std::endl<<std::endl;  //-->IT DOESN'T PRINT STATEMENT
	jack_default_audio_sample_t *out1, *out2;
	paTestData *data = (paTestData*)arg;
	int i;

	out1 = (jack_default_audio_sample_t*)jack_port_get_buffer (output_port3, nframes);
	out2 = (jack_default_audio_sample_t*)jack_port_get_buffer (output_port4, nframes);

    //std::cout<<"nframes = "<<nframes<<std::endl;
	for( i=0; i<nframes; i++ )
	{
		out1[i] = data->sine[data->left_phase];  /* left */
		out2[i] = data->sine[data->right_phase];  /* right */
		data->left_phase += 1;
		if( data->left_phase >= TABLE_SIZE ) data->left_phase -= TABLE_SIZE;
		data->right_phase += 7; /* higher pitch so we can distinguish left and right. */
		if( data->right_phase >= TABLE_SIZE ) data->right_phase -= TABLE_SIZE;
	}

	return 0;
}

int main(){
	paTestData data;
}

/* install a signal handler to properly quits jack client */
#ifdef WIN32
	signal(SIGINT, signal_handler);
	signal(SIGABRT, signal_handler);
	signal(SIGTERM, signal_handler);
#else
	signal(SIGQUIT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGHUP, signal_handler);
	signal(SIGINT, signal_handler);
#endif

#endif // BEEPER_H_INCLUDED
