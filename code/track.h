#ifndef TRACK_H_INCLUDED
#define TRACK_H_INCLUDED

//######################################################################################################################
//FORWARD DECLARATIONS
#define JACK_DEFAULT_AUDIO_TYPE   "32 bit float mono audio"
/*enum JackPortFlags{JackPortIsInput = 0x1,
                    JackPortIsOutput = 0x2,
                    JackPortIsPhysical = 0x4,
                    JackPortCanMonitor = 0x8,
                    JackPortIsTerminal = 0x10
};*/
typedef struct _jack_port jack_port_t;
typedef struct _jack_client jack_client_t;
typedef uint32_t jack_nframes_t;
#ifdef __cplusplus
extern "C" {
#endif

jack_port_t* jack_port_register(jack_client_t *client,
                                const char *port_name,
                                const char *port_type,
                                unsigned long flags,
                                unsigned long buffer_size);
int jack_port_disconnect(jack_client_t *,jack_port_t *);
const char *jack_port_name(const jack_port_t *port);
int jack_port_connected(const jack_port_t * port);
void* jack_port_get_buffer(jack_port_t *,jack_nframes_t);
#ifdef __cplusplus
}
#endif
//#####################################################################################################################
//######################################################################################################################

class track{
public:
    track();
    ~track();

    jack_port_t* get_input_port() { return tp.input_port; }
    jack_port_t* get_output_port_left() { return tp.output_port_left; }
    jack_port_t* get_output_port_right() { return tp.output_port_right; }
    jack_port_t* get_mono_port() { return tp.mono_port; }
    //for monophonic output?

    void register_input_ports(jack_client_t *client,unsigned long flag);
    void register_output_ports(jack_client_t *client,unsigned long flag);

    void mute(jack_client_t *client); //virtual
    void playback(/*...*/);
    void record(/*...*/);
    void overdub(/*...*/);
    int monitor(uint32_t nframes);
    const char* get_uniqname(){return uniqname;}
//private:
    void set_uniqname();//internall proccess - virtual
    track_port tp;
    void mute_input_ports(jack_client_t *client);//instrunment
    void mute_output_ports(jack_client_t *client);//both instrunment and digital
    const char* uniqname;
};


#endif // TRACK_H_INCLUDED
