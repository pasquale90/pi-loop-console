#ifndef CHANNEL_H_INCLUDED
#define CHANNEL_H_INCLUDED

#include <atomic>
#include "audio_settings.h"

#define MIN_LOOP_LENGTH_SAMPLES 1*SAMPLE_RATE //that is 1 second min length of the loop
#define MAX_LOOP_LENGTH_SAMPLES 37*SAMPLE_RATE //that is 37 seconds max length of the loop

// + consider master channel

class Channel{

    public:
        Channel();
        // Channel(const char*);
        void update_signal(float*[F_NUM_INPUTS]);

// EVENTS FOR THOSE IN CHANNEL 
// CH1_VOL_LOW,
// CH1_VOL_HIGH,
// CH2_VOL_LOW,
// CH2_VOL_HIGH,
// CH3_VOL_LOW,
// CH3_VOL_HIGH 

        void recdub();
        void undub();
        void pause();
        void reset();

    private:
        const char* channel_name;

        static const int num_states{2};
        float data[num_states][MAX_LOOP_LENGTH_SAMPLES];

        int currSize;
        std::atomic<int> rec_id; // points to the current or the previous recording , currently we have 2
        std::atomic<bool> isempty; // if nothing has been written yet
        std::atomic<bool> start; // start/pause looping/overdubing
};

#endif