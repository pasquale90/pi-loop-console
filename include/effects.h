#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED

#include <random>

const int NUM_EFFECTS = 3;

class Effects{
    public:
        Effects();
        
        void apply_effect(int,float*);
        void apply_effects(bool[NUM_EFFECTS],float*);
        
    private:
        void _whiteNoise(float*);
        void _effect2(float*);
        void _effect3(float*);
    
};
#endif