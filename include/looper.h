#ifndef LOOPER_H_INCLUDED
#define LOOPER_H_INCLUDED

#include "channel.h"
// #include "config.h"
#include "metronome.h"
#include "response.h"

/*! @brief Looper output buffers type alias.
  */
using LooperOutput = std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS>;

/*! @brief The Looper class constisted of 3 loop-channels.*/
class Looper{
    public:
        /*! @brief Class contructor.
         */
        Looper();

        /*! @brief Function to do the tap-tempo or to change the metronome to an alternative value of tempo and number of measures.
         * @param bool - if the button is pressed consecutively. 
         * if true, then it alters the tempo. Otherwise it is a tap-tempo signal.
         * @return void
         */
        void tap_alter_metronome(bool);

        /*! @brief Method that receives the input buffer signal from the monitor.
         * @param float*[F_NUM_INPUTS] - the input buffer sent by the monitor.
         * @param bool[F_NUM_INPUTS] -  the arm states
         * @return std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> -  the looper ouput buffer
         */
        LooperOutput* update_buffer(float*[F_NUM_INPUTS],bool[F_NUM_INPUTS]);

        /*! @brief Function triggered by the REC button. Supports four (4) operations : record-dub/stop_recording/playback/erase_previous
         * Depending on the current state of the looper, it decides which one of them to apply.
         * @param int - the related channel
         * @param bool - whether the button was pressed consecutively or not. If yes, it is an undub signal. Otherwise it is one among the rest of the operations.
         * @param Response& - the output response that is used to update the display state of the looper.
         * @return void
         */
        void recdub(int,bool,Response&);

        /*! @brief Function triggered by the PLAY/STOP button. Supports three (3) operations : stop_playback/stop_recording/erase_channel
         * Depending on the current state of the looper, it decides which one of them to apply.
         * @param int - the related channel
         * @param bool - whether the button was pressed consecutively or not. If yes, it is an erase signal. Otherwise it is one among the rest of the operations.
         * @param Response& - the output response that is used to update the display state of the looper.
         * @return void
         */
        void stoperase(int,bool,Response&);

        /*! @brief Function triggered by the START_ALL button. Supports two operations : start_stop_all/ erase_all
         * Depending on the current state of the looper, it decides which one of them to apply.
         * @param bool - whether the button was pressed consecutively or not. If yes, it is an erase-all-channels signal. 
         * Otherwise it is a signal that aims to pause all channels or to start playing all channels
         * @param Response& - the output response that is used to update the display state of the looper.
         * @return void
         */
        void start_stop_all(bool,Response&);

        /*! @brief Sets a channel's volume with a specific value
         * @param int - the corresponding channel
         * @param int - the value of the volume as type of integer percentage.
         * @return void
         */
        void volume_change(int,int);

        /*! @brief Increase the channel's volume by a channels-defined volume step. 
         * Used for when changing volumes with keyboard keys.
         * @param int - the corresponding channel
         * @return void
         */
        void volume_up(int);

        /*! @brief Decrease the channel's volume by a channels-defined volume step. 
         * Used for when changing volumes with keyboard keys.
         * @param int - the corresponding channel
         * @return void
         */
        void volume_down(int);
        
        /*! @brief Save the looper state. Unsupported yet.
         * @return void
         */
        bool save();

        /*! @brief Display the looper's current state. Used for debugging.
         * @return void
         */
        void display_states();
        
        /*! @brief Function used to update the metronome's display status.
         * @return int - the intonation value of the metronome.
         */
        int get_metronome_state();

        /*! @brief Resets both the looper and the channels attached to it to the industrial conditions!
         * (i.e. used for when changing session)
         * @return void
         */
        void reset();

    private:

        Metronome metronome;
        static const int num_channels{3};
        Channel channels[num_channels];
        // Channel ch1{"LoopCh1"},ch2{"LoopCh2"},ch3{"LoopCh3"};
        // ch1{"LoopCh1"},ch2{"LoopCh2"},ch3{"LoopCh3"};
        
        std::atomic<bool> arm_enabled;
        // std::atomic<bool> arm_enabled[F_NUM_INPUTS], at_least_one_arm;
        std::atomic<int> loop_length, playback_idx;
        std::atomic<bool> playback[num_channels]; //,record[num_channels];
        std::atomic<int> record; // recording is mutually exclusive, playback is not.
        std::atomic<int> ticktock;
        LooperOutput looper_output;

        /*! @brief Resets the looper only. Not the channels. Is is also used for initializing the looper.
         * @return void
         */
        void _set_inactive();

        /*! @brief function used to fuse the input signal depending on the ARM states.
         * This allows the looper to treat all input signals as if is is one signal, as long as their ARM state is enabled.
         * @param const float* const[F_NUM_INPUTS] - the input signal of all input channels
         * @param bool[F_NUM_INPUTS] - the ARM states of all input channels.
         * @param float[BUFFER_SIZE] - the fused output signal passed by reference.
         * @return bool -  if at least one ARM is enabled. Otherwise, false to avoid unnecessary work.
         */
        bool _fuse_input(const float* const[F_NUM_INPUTS], bool[F_NUM_INPUTS], float[BUFFER_SIZE]);
        
        /*! @brief function to initialize the looper with the loop length defined by the first recording.
         * It also sets the metronome.
         * @return void
         */
        void _initialize_looper();

        /*! @brief function to the looper's output buffer.
         * @return std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> - the looper's output buffer.
         */
        LooperOutput* mix(); // mix all channels before updating mixer

        /*! @brief function used to update the Response message of the looper.
         * This is used to update the display of the looper's output state.
         * @param Response& - the response message passed by reference.
         * @return void
         */
        void _update_response(Response&);
        
};

#endif