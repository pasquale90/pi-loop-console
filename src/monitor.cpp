#include "monitor.h"

int callback_f (jack_nframes_t nframes,void *arg){
    (void)nframes; //!supress_warning unused parameter
    return static_cast<Monitor*>(arg)->process();
}

int Monitor::process()
{
    /**
     * The process callback for this JACK application is called in a
     * special realtime thread once for each audio cycle.
     */


        for (int i=0; i<F_NUM_INPUTS;++i){
// @TODO Do not run if all monitor states disabled. Pass only the channel that has at least arm || mntr enabled.
            input_buffers[i] = hs.get_input_buffer(i);
        }

        
        effects.apply(input_buffers);


        for (int i=0; i<F_NUM_OUTPUTS;++i){
            output_buffers[i] = hs.get_output_buffer(i);
        }

        stream_buffer(input_buffers,output_buffers);
        
    
	return 0;      
}

Monitor::Monitor(){
}

void Monitor::set_stream_buffer(std::function<void(float *in[F_NUM_INPUTS],float *out[F_NUM_OUTPUTS])> streamBufferf){
    stream_buffer = streamBufferf;
}

void Monitor::initialize_effects(const bool effects_curr_state[F_NUM_INPUTS][NUM_EFFECTS]){
    // for (int i=0; i<F_NUM_INPUTS;++i){
    //     for (int j = 0; j < NUM_EFFECTS; j++)
    //         effects_enabled[i][j].store(effects_curr_state[i][j]);
    // }
    effects.initialize_effects(effects_curr_state);
}

bool Monitor::toggle_effect(int ch,int eff){
    return effects.toggle_effect(ch, eff);
    // effects_enabled[ch][eff].store(val) ; //!effects_enabled[ch][eff];
}

void Monitor::connect(char *name){
    hs.link_client(name);
    hs.set_process_callback();
    hs.prevent_failure();

    for (int i=0; i<F_NUM_INPUTS; ++i)
        hs.register_input_port(i);

    for (int i=0; i<F_NUM_OUTPUTS; ++i)
        hs.register_output_port(i);

// change server operations
    hs.set_buffer_size();

// @TODO latency_ms

    hs.activate();
    hs.register_devices();

    for (int i=0; i<F_NUM_INPUTS; ++i){
        hs.connect_input_device(i);
    }
    for (int i=0; i<F_NUM_OUTPUTS; ++i){
        hs.connect_output_device(i);
    }

#ifdef VERBOSE_MONITOR
    hs.check_status(); // default value : verbose = false;
#endif
}

void Monitor::disconnect(){
    for (int i=0; i<F_NUM_OUTPUTS; ++i){
        mute_output(i);
        hs.disconnect_output_device(i);
    }
    std::cout<<"\n\nMonitor::Output devices disconnected succesfully \n\n"<<std::endl;
    
    for (int i=0; i<F_NUM_INPUTS; ++i){
        mute_input(i);
        hs.disconnect_input_device(i);
    }
    std::cout<<"\n\nMonitor::Input devices disconnected succesfully \n\n"<<std::endl;

    hs.disconnect_client();
}

void Monitor::mute_input(int device){
    hs.disconnect_input_device(device);
}

void Monitor::mute_output(int device){
    hs.disconnect_output_device(device);
}

void Monitor::update_states(bool is_mntr, int input_channel, bool val){
    input_enabled[int(is_mntr)][input_channel].store(val);
}

bool Monitor::toggle_states(bool is_mntr, int channel){
    // return input_enabled[int(is_mntr)][channel].exchange(!input_enabled[int(is_mntr)][channel].load()) ;
    input_enabled[int(is_mntr)][channel].store(!input_enabled[int(is_mntr)][channel].load()) ;
    return input_enabled[int(is_mntr)][channel].load();
}

void Monitor::get_states(bool states[F_NUM_INPUTS], bool is_mntr){
    
    for (int chin=0; chin < F_NUM_INPUTS; ++chin)
        states[chin] = input_enabled[int(is_mntr)][chin].load();

}

/*
void Monitor::mute_microphone()
{   
    hs.disconnect_input_device(0);
}

void Monitor::unmute_microphone()
{   
    hs.connect_input_device(0);
}

void Monitor::mute_instrument()
{
#if INST   
    hs.disconnect_input_device(1);
#else
    std::cout<<"Monitor::mute_instrument --> No INST input supported for "<<DEV_INFO<<std::endl;
#endif
}

void Monitor::unmute_instrument()
{   
#if INST
    hs.connect_input_device(1);
#else
    std::cout<<"Monitor::unmute_instrument --> No INST input supported for "<<DEV_INFO<<std::endl;
#endif
}
*/