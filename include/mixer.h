#ifndef MIXER_H_INCLUDED
#define MIXER_H_INCLUDED

#include "audio_settings.h"

#include <iostream>
#include <cstring>
#include "config.h"

// @TODO consider making Mixer a function

class Mixer{
  public:
    Mixer();

    // void update_buffer(float*[F_NUM_INPUTS],float*[F_NUM_OUTPUTS],float*[F_NUM_OUTPUTS],bool[F_NUM_INPUTS]);
    void update_buffer(float*[F_NUM_INPUTS],float*[F_NUM_OUTPUTS],std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS>,bool[F_NUM_INPUTS]);

    void save_jam();
    
  private:
    Config& cfg = Config::getInstance();
    char* name;
};

#endif