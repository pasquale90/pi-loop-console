#ifndef CHANNEL_H_INCLUDED
#define CHANNEL_H_INCLUDED

#include <array>
#include <vector>

#include "audio_settings.h"

#define MAX_LOOP_LENGTH_SAMPLES 37*SAMPLE_RATE //that is 37 seconds max length of the loop

// @TODO consider stereo operations for looper class.

static const int volumeStep = 10;

/*! @brief Defines a looper-channel designed to loop tracks.*/
class Channel{

    public:
        /*! @brief default contructor
        */
        Channel();
        
        /*! @brief function used to clear all data written on the channels.
        * @return void
        */
        void reset();

        /*! @brief function used to set a name on the current channel.
        * @return void
        */
        void set_name(const char*);

        /*! @brief function used to get the name of the current channel.
        * @return void
        */
        const char* get_name();

        /*! @brief function used to add a new track on the current channel. 
         * In fact, it only stores the information that the track is saved.
         * @return void
        */
        void add_track();

        /*! @brief sets num tracks to zero.
        * @return void
        */
        void reset_num_tracks();

        /*! @brief gets the number of tracks of this channel.
        * @return int - the number of tracks
        */
        int get_num_tracks();
        
        /*! @brief function to pass the current buffer to rec or to dub. 
        * This function is only called when there is work to be done. 
        * @return void
        */
        void update_rec_signal(float[BUFFER_SIZE], int);


        /*! @brief gets the volume of the current channel
        * @return float - the volume as type of a float percentage
        */
        float get_volume();
        
        /*! @brief sets the volume for the current channel
        * @param int - the volume as type of an int percentage
        * @return void
        */
        void set_volume(int);


        void volume_up();
        void volume_down();        

        void clean();
        void rec(float[BUFFER_SIZE],int);
        void dub(float[BUFFER_SIZE],int);
        void undub();
        
        /*! @brief function to return the channels output signal
        * @param int - the index that indicates the current playback position
        * @return std::vector<float> -  the output buffer
        */
        std::vector<float> get_out_signal(int);
        
        /*! @brief function to return whether the channel has recorded tracks or not
        * @return bool - true if has tracks, otherwise false.
        */
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