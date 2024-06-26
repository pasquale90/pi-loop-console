#ifndef LOOPER_H_INCLUDED
#define LOOPER_H_INCLUDED

#include "channel.h"
// #include "config.h"
#include "metronome.h"

class Looper{
    public:
        Looper();

        void load_metronome();
        void tap_alter_metronome(bool);

        // float** update_buffer(float*[F_NUM_INPUTS],bool[F_NUM_INPUTS]);
        std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> update_buffer(float*[F_NUM_INPUTS],bool[F_NUM_INPUTS]);

        void recdub(int,bool);
        void stoperase(int,bool);

        void volume_up(int);
        void volume_down(int);
        
        bool save();
        void start_stop_all(bool);
        void display_states();

        void reset();
    private:
void _set_inactive();

        Metronome metronome;
        static const int num_channels{3};
        Channel channels[num_channels];
        // Channel ch1{"LoopCh1"},ch2{"LoopCh2"},ch3{"LoopCh3"};
        // ch1{"LoopCh1"},ch2{"LoopCh2"},ch3{"LoopCh3"};
        
        std::atomic<bool> arm_enabled;
        std::atomic<int> loop_length, playback_idx;
        std::atomic<bool> playback[num_channels]; //,record[num_channels];
        std::atomic<int> record; // recording is mutually exclusive, playback is not.
        
        bool _fuse_input(const float* const[F_NUM_INPUTS], bool[F_NUM_INPUTS], float[BUFFER_SIZE]);
        void _initialize_looper();
        std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> mix(); // mix all channels before updating mixer
        
};

#endif