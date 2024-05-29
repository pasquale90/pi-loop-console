#ifndef MONITOR_H_INCLUDED
#define MONITOR_H_INCLUDED

#include "handshake.h"
#include <functional>
using namespace std::placeholders;

// #include "effects.h"
class Session;

class Monitor{
    public:
        Monitor();
        
        void connect(char*,bool[F_NUM_INPUTS]);
        void disconnect();

        void mute_microphone();
        void unmute_microphone();
        void mute_instrument();
        void unmute_instrument();

        int process();
        void set_stream_buffer(std::function<void(float *[F_NUM_INPUTS],float *[F_NUM_OUTPUTS])>);

    private:
        Handshake hs;
        // Effects effects;

        float *input_buffers[F_NUM_INPUTS];
        float *output_buffers[F_NUM_OUTPUTS];

        std::function<void(float *[F_NUM_INPUTS],float *[F_NUM_OUTPUTS])> stream_buffer;


};

#endif