#include "looper.h"
#include <iostream>

float** Looper::update_buffer(float* input[F_NUM_INPUTS]){

    for (int i=0; i<num_channels;++i){
        channels[i].update_signal(input);
    }
// EVENTS FOR THOSE
// CH1_RECDUB=1,
// CH1_STOP,
// CH2_RECDUB,
// CH2_STOP,
// CH3_RECDUB,
// CH3_STOP,
    
    return mix();
}

void Looper::recdub(int channel,bool isHold){
    if (isHold)
        channels[channel].undub();
    else
        channels[channel].recdub();
}

void Looper::stoperase(int channel,bool isHold){
    if (isHold)
        channels[channel].reset();
    else
        channels[channel].pause();
}

bool Looper::save(){

}

void Looper::set_loop_length(){

}

float** Looper::mix(){

    float *looper_output[F_NUM_OUTPUTS] = {0};


    // for (channel) || for (0;BUFFER_SIZE;++)
    //   looper_output[][] = ..
    //   looper_output[][] = ..

    return looper_output;

}