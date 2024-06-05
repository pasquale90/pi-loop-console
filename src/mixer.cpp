#include "mixer.h"
#include "audio_settings.h"

Mixer::Mixer(){
}


// void Mixer::update_buffer(float* input_buffers[F_NUM_INPUTS],float* output_buffers[F_NUM_OUTPUTS], float* looper_buffers[F_NUM_OUTPUTS],bool monitorIn[F_NUM_INPUTS]){
void Mixer::update_buffer(float* input_buffers[F_NUM_INPUTS],float* output_buffers[F_NUM_OUTPUTS], std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> looper_buffers,bool monitorIn[F_NUM_INPUTS]){
   
    for (int i=0;i<BUFFER_SIZE;++i){
        
        // for (int j = 0 ; j < F_NUM_OUTPUTS; ++j )

#if F_NUM_INPUTS == 1
        //L
           output_buffers[0][i] = input_buffers[0][i] * (int)monitorIn[0] + looper_buffers[0][i];
        //R
            output_buffers[1][i] = input_buffers[0][i] * (int)monitorIn[0] + looper_buffers[1][i];
#elif F_NUM_INPUTS == 2
        //L
            output_buffers[0][i] = input_buffers[0][i] * (int)monitorIn[0] + input_buffers[1][i] * (int)monitorIn[1] + looper_buffers[0][i];
        //R
            output_buffers[1][i] = input_buffers[0][i] * (int)monitorIn[0] + input_buffers[1][i] * (int)monitorIn[1] + looper_buffers[1][i];
#endif
    }

// @TODO - > DONE!
    // Mix all signals
        // Current monitor (2 inputs max) if monitor enabled
        // Looper playback (a mixed signal / or 3 separate buffers)

// @TODO compression
}

void Mixer::save_jam(){
    // save last 5 min output before streaming to speakers
    // set PREPROCESSOR VALUE max_size_samples = SR* (5min*60seconds/minute)
}

// void Mixer::turnOff(){
//     // hs.stop_running();
// }
