#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED

#include <random>

class Effects{
    public:
        Effects();
        
        void whiteNoise(float*);
        void effect2(float*);
        void effect3(float*);
    
    private:
        int buffer_size;
};
#endif