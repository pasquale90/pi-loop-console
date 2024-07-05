#include "effects.h"
#include "audio_settings.h"

Effects::Effects(){
    
    // effect 1

    // effect 2

    // effect 3
        
}

void Effects::initialize_effects(const bool effects_curr_state[F_NUM_INPUTS][NUM_EFFECTS]){
    for (int i=0; i<F_NUM_INPUTS;++i){
        for (int j = 0; j < NUM_EFFECTS; j++)
            effects_enabled[i][j].store(effects_curr_state[i][j]);
    }
}

void Effects::toggle_effect(int ch,int eff,bool val){
    effects_enabled[ch][eff].store(val) ; //!effects_enabled[ch][eff];
}

void Effects::apply(float *input_buffers[F_NUM_INPUTS]){

// @TODO complexity improvement using this loop
    /*
    for (int i = 0; i<F_NUM_INPUTS; ++i){

        for (int j = 0; j<BUFFER_SIZE; ++j){
            
            for (int eff = 0; eff < NUM_EFFECTS; ++eff){                

                if (effects_enabled[i][eff].load()) {
                    
                    // contribute to the final sig at pos i,j
                
                }

            }

        }

    }
    */

   // for now naive solution
   for (int i = 0; i<F_NUM_INPUTS; ++i){

        if (effects_enabled[i][0].load()) 
            _apply_whiteNoise(input_buffers[i]);

        if (effects_enabled[i][1].load())   
            _apply_effect2(input_buffers[i]);

        if (effects_enabled[i][2].load())
            _apply_effect3(input_buffers[i]);

    }
}


float* Effects::_apply_whiteNoise(float *in)
{
   for (int i=0; i<BUFFER_SIZE; ++i) {
        float x = in[i] * 0.005; // * 0.005 to reduce noise
        in[i] = (x / abs(x)) * ( 1. - std::pow ( std::exp(1) , std::pow(x,2)/ abs(x)) );
    }
}

float* Effects::_apply_effect2(float *in){
    // delay ?    
}



float* Effects::_apply_effect3(float *in){
    // metro-synched wah ?
}