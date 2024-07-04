#ifndef CHANNEL_H_INCLUDED
#define CHANNEL_H_INCLUDED

// #include <atomic>
#include <array>
#include <vector>

#include "audio_settings.h"

#define MAX_LOOP_LENGTH_SAMPLES 37*SAMPLE_RATE //that is 37 seconds max length of the loop

// @TODO consider stereo operations for looper class.

static const int volumeStep = 10;

class Channel{

    public:
        Channel();
        
        void reset();

        void set_name(const char*);
        const char* get_name();

        void add_track();
        void reset_num_tracks();
        int get_num_tracks();
        
        void update_rec_signal(float[BUFFER_SIZE], int);

        float get_volume();
        void volume_up();
        void volume_down();        

        void clean();
        void rec(float[BUFFER_SIZE],int);
        void dub(float[BUFFER_SIZE],int);
        void undub();
        
        // float* get_out_signal(int);
        std::vector<float> get_out_signal(int);
        
        bool isEmpty();
    private:
        const char* channel_name;
        int volume;

        static const int num_states{2}; // 2 states : the current state and the previous state
        
        std::array< std::vector<float> , num_states > data;
        
        int states_avail[num_states];
        int num_tracks,rec_id,play_id;

        int volume_step;
};

#endif