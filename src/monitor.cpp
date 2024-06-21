#include "monitor.h"

int callback_f (jack_nframes_t nframes,void *arg){
    return static_cast<Monitor*>(arg)->process();
}

int Monitor::process()
{
    /**
     * The process callback for this JACK application is called in a
     * special realtime thread once for each audio cycle.
     */

    for (int i=0; i<F_NUM_INPUTS;++i){
        input_buffers[i] = hs.get_input_buffer(i);        
        for (int eff = 0; eff<NUM_EFFECTS; ++eff){
            if (effects_enabled[i][eff].load()){
// @TODO improve this complexity
                effects.apply_effect(eff, input_buffers[i]);
            }
        }
    }

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
    for (int i=0; i<F_NUM_INPUTS;++i){
        for (int j = 0; j < NUM_EFFECTS; j++)
            effects_enabled[i][j].store(effects_curr_state[i][j]);
    }
}

void Monitor::toggle_effect(int ch,int eff,bool val){
    effects_enabled[ch][eff].store(val) ; //!effects_enabled[ch][eff];
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

/*
//@TODO optimize this function with preproccessor if statements blocks
void Monitor::_set_input_buffers(){

    // // if (cfg.device_settings.micIn && !cfg.button_states[IN1_ARM] && cfg.button_states[IN1_MNTR]){
    
    // bool mic_condition = cfg.device_settings.micIn && (cfg.button_states[IN1_ARM] || cfg.button_states[IN1_MNTR]);
    // if (mic_condition){
    //     mic_buffer = hs.get_mic_buffer();
    //     if (cfg.get_button_state(IN1_EFF1)){
    //         effects.whiteNoise(mic_buffer);
    //     }
    //     if (cfg.get_button_state(IN1_EFF2)){
    //         effects.effect2(mic_buffer);
    //     }        
    //     if (cfg.get_button_state(IN1_EFF3)){
    //         effects.effect3(mic_buffer);
    //     }
    // }else mic_buffer = nullptr;

    // bool inst_condition = cfg.device_settings.instIn && (cfg.button_states[IN2_ARM] || cfg.button_states[IN2_MNTR]);
    // // if (cfg.device_settings.instIn && !cfg.button_states[IN2_ARM] && cfg.button_states[IN2_MNTR]){
    // if (inst_condition){
    //     inst_buffer = hs.get_inst_buffer();

    //     // if (!cfg.button_states[IN2_ARM] && cfg.button_states[IN2_MNTR]){}
    //     if (cfg.get_button_state(IN2_EFF1)){
    //         effects.whiteNoise(inst_buffer);
    //     }

    //     if (cfg.get_button_state(IN2_EFF2)){
    //         effects.effect2(inst_buffer);
    //     }
        
    //     if (cfg.get_button_state(IN2_EFF3)){
    //         effects.effect3(inst_buffer);
    //     }
    // }else inst_buffer = nullptr;

    for (int i =0; i< F_NUM_INPUTS; ++i)
        input_buffers[i] = hs.get_input_buffer(i);
    // inst_buffer = hs.get_inst_buffer();
}
*/