#ifndef MIXER_H_INCLUDED
#define MIXER_H_INCLUDED

#include "audio_settings.h"
#include "AudioFile_cxx11.h"

#include <array>
#include <atomic>

// @TODO conform to both mono and stereo out

/*! @brief The mixer module responsible to stream the output signal.*/
class Mixer{
  public:
    /*! @brief Class contructor.
     */
    Mixer();

    /*! @brief Function that streams the output audio signal
     * @param float*[F_NUM_INPUTS] - the monitor audio input signal
     * @param float*[F_NUM_OUTPUTS] - the output buffers connected to the speakers
     * @param std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> - the looper output buffers
     * @param bool[F_NUM_INPUTS] - bool array to indicate if monitorIn is enabled.
     * @return void
     */
    void update_buffer(float*[F_NUM_INPUTS],float*[F_NUM_OUTPUTS],std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS>,bool[F_NUM_INPUTS]);

    /*! @brief Function that saves the last minutes to a file
     * @param std::string - the path to save the file.
     * @return void
     */
    void save_jam(std::string);
    
  private:
    char* name;

    std::atomic<int> cnt,curr_max_size; 
    AudioFile<float> audioFile;
    AudioFile<float>::AudioBuffer jam_buffer;
};

#endif