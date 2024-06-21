#ifndef CHANNEL_H_INCLUDED
#define CHANNEL_H_INCLUDED

// #include <atomic>
#include <array>
#include <vector>
#include <atomic>

#include "audio_settings.h"

// #define MIN_LOOP_LENGTH_SAMPLES 1*SAMPLE_RATE //that is 1 second min length of the loop
// #define MAX_LOOP_LENGTH_SAMPLES 37*SAMPLE_RATE //that is 37 seconds max length of the loop

// #define DEBUGLOOP
// @TODO consider stereo operations for looper class.
// @TODO optimize channel operations

// + consider master channel

static const int volumeStep = 10;

class Channel{

    public:
        Channel();
        
        // void set_first_loop_mode();
        void reset();

        void set_name(const char*);
        const char* get_name();

        void set_loop_length(bool,int);
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
        //  void debug();
    private:
        const char* channel_name;
        int volume;

        static const int num_states{2}; // 2 states : the current state and the previous state
        
        // float data[num_states][F_NUM_OUTPUTS][MAX_LOOP_LENGTH_SAMPLES];   
        std::array< std::vector<float> , num_states > data;
        
        int states_avail[num_states];
        int num_tracks,rec_id,play_id;

        int volume_step;

        std::atomic<int> loop_length;
};

#endif