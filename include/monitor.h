#ifndef MONITOR_H_INCLUDED
#define MONITOR_H_INCLUDED

#include "handshake.h"
#include "effects.h"
#include <functional>

class Session;

class Monitor{
    public:
        Monitor();

        void connect(char*);
        void disconnect();

        void initialize_effects(const bool[F_NUM_INPUTS][NUM_EFFECTS]);
        void toggle_effect(int,int,bool);
        
        void mute_input(int);
        void mute_output(int);
        // void mute_microphone();
        // void unmute_microphone();
        // void mute_instrument();
        // void unmute_instrument();

        int process();
        void set_stream_buffer(std::function<void(float *[F_NUM_INPUTS],float *[F_NUM_OUTPUTS])>);

    private:
        Handshake hs;
        Effects effects;
        bool effects_enabled[F_NUM_INPUTS][NUM_EFFECTS];

        float *input_buffers[F_NUM_INPUTS];
        float *output_buffers[F_NUM_OUTPUTS];

        std::function<void(float *[F_NUM_INPUTS],float *[F_NUM_OUTPUTS])> stream_buffer;


};

#endif