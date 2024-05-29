#ifndef LOOPER_H_INCLUDED
#define LOOPER_H_INCLUDED

#include "channel.h"
#include "config.h"
#include "metronome.h"

class Looper{
    public:
        float** update_buffer(float*[F_NUM_INPUTS]);

        void recdub(int,bool);
        void stoperase(int,bool);

        bool save();

        void set_loop_length(); // logic for setting the metronome and setting the range for the palyback pointer

    private:
        Config& cfg = Config::getInstance();
        int loop_length;
        
        static const int num_channels{3};
        Channel channels[num_channels];
        
        float** mix(); // mix all channels before updating mixer
        
        // Channel ch1{"LoopCh1"},ch2{"LoopCh2"},ch3{"LoopCh3"};
        // ch1{"LoopCh1"},ch2{"LoopCh2"},ch3{"LoopCh3"};
};

#endif