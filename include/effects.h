#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED

#include <iostream>
#include <cmath>
#include <atomic>
#include "audio_settings.h"
// #include "config.h"

const int NUM_EFFECTS = 3;

class Effects{
    public:
        Effects();
        
        void initialize_effects(const bool[F_NUM_INPUTS][NUM_EFFECTS]);
        void toggle_effect(int,int,bool);

        void apply(float*[F_NUM_INPUTS]);
        
    private:

        // Config& cfg = Config::getInstance();
        
        std::atomic<bool> effects_enabled[F_NUM_INPUTS][NUM_EFFECTS];

        float* _apply_whiteNoise(float*);
        float* _apply_effect2(float*);
        float* _apply_effect3(float*);

};  
#endif