#ifndef MIXER_H_INCLUDED
#define MIXER_H_INCLUDED

#include "audio_settings.h"
#include "AudioFile_cxx11.h"

#include <array>
#include <atomic>

// @TODO conform to both mono and stereo out

class Mixer{
  public:
    Mixer();

    // void update_buffer(float*[F_NUM_INPUTS],float*[F_NUM_OUTPUTS],float*[F_NUM_OUTPUTS],bool[F_NUM_INPUTS]);
    void update_buffer(float*[F_NUM_INPUTS],float*[F_NUM_OUTPUTS],std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS>,bool[F_NUM_INPUTS]);

    void save_jam(std::string);
    
  private:
    char* name;

    std::atomic<int> cnt,curr_max_size; 
    AudioFile<float> audioFile;
    AudioFile<float>::AudioBuffer jam_buffer;
};

#endif