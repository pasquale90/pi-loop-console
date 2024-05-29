#include "mixer.h"
#include "audio_settings.h"

Mixer::Mixer(){
}


void Mixer::update_buffer(float* input_buffers[F_NUM_INPUTS],float* output_buffers[F_NUM_OUTPUTS], float* looper_buffers[F_NUM_OUTPUTS]){

    // if (cfg.get_button_state()){
    // }else{
    // }    

    // Check these here
    // IN1_ARM,
    // IN1_MNTR,
    // IN2_ARM,
    // IN2_MNTR,

    // IN1_EFF1,
    // IN1_EFF2,
    // IN1_EFF3,
    // IN2_EFF1,
    // IN2_EFF2,
    // IN2_EFF3,

    // std::cout<<
//EVENTS FOR THOSE
// START_ALL,
// SAVE_JAM,

// Mix signals here
    // if (cfg.get_button_state(IN1_ARM)){
    for (int i=0;i<BUFFER_SIZE;++i){
        
        for (int j = 0 ; j < F_NUM_OUTPUTS; ++j )

#if F_NUM_INPUTS == 1
        //L
           output_buffers[0][i] = input_buffers[0][i];
        //R
            output_buffers[1][i] = input_buffers[0][i];
#elif F_NUM_INPUTS == 2
        //L
            output_buffers[0][i] = input_buffers[0][i] + input_buffers[1][i];
        //R
            output_buffers[1][i] = input_buffers[0][i] + input_buffers[1][i];
#endif
    }
    // memcpy (output_buffers[0], input_buffers[0] ,sizeof (float) * BUFFER_SIZE);
    // memcpy (output_buffers[1], input_buffers[0] ,sizeof (float) * BUFFER_SIZE);

// @TODO
    // Mix all signals
        // Current monitor (2 inputs max) if monitor enabled
        // Looper playback (a mixed signal / or 3 separate buffers)
}

void Mixer::start_stop_all(){
    // create an atomic variable to stop.
    // set channels to indicate at the first position of the ring buffer
}

void Mixer::save_jam(){
    // save last 5 min output before streaming to speakers
    // set PREPROCESSOR VALUE max_size_samples = SR* (5min*60seconds/minute)
}

// void Mixer::turnOff(){
//     // hs.stop_running();
// }

// void mix_output(){

    
// }