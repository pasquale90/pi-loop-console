#include "effects.h"
#include "audio_settings.h"

Effects::Effects(){
}

void Effects::apply_effects(bool enabled_effects[NUM_EFFECTS],float* signal){

//update signal with effects, if at least of the effects is enabled

    // for (int i=0;i<NUM_EFFECTS;++i){
    //     if (enabled_effects[i])
    //          ...
    // }

// @TODO improve this complexity
    if (enabled_effects[0])
        _whiteNoise(signal);
    if (enabled_effects[1])
        _effect2(signal);
    if (enabled_effects[2])
        _effect3(signal);
}

void Effects::apply_effect(int effect,float* sig){

// @TODO improve this complexity. Make 1 traversal instead?
    if (effect==0)
        _whiteNoise(sig);
    if (effect==1)
        _effect2(sig);
    if (effect==2)
        _effect3(sig);
}


void Effects::_whiteNoise(float *in)
{
   for (int i=0; i<BUFFER_SIZE; ++i) {
        float x = in[i] * 0.005; // * 0.005 to reduce noise
        in[i] = (x / abs(x)) * ( 1. - std::pow ( std::exp(1) , std::pow(x,2)/ abs(x)) );
    }

}

void Effects::_effect2(float *in){
    // delay ?
}


void Effects::_effect3(float *in){
    // metro-synched wah ?
}