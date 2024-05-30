#ifndef MIXER_H_INCLUDED
#define MIXER_H_INCLUDED

#include "audio_settings.h"

#include <iostream>
#include <cstring>
#include "config.h"
// #include "channel.h"


class Mixer{
  public:
    Mixer();

    void update_buffer(float*[F_NUM_INPUTS],float*[F_NUM_OUTPUTS],float*[F_NUM_OUTPUTS],bool[F_NUM_INPUTS]);

    void start_stop_all();
    void save_jam();
    
  private:
    Config& cfg = Config::getInstance();
    char* name;
};

#endif