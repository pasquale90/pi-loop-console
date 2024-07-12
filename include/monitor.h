#ifndef MONITOR_H_INCLUDED
#define MONITOR_H_INCLUDED

#include "handshake.h"
#include "effects.h"
#include <functional>

/*! @brief The monitor module encapsulates the jack audio client.*/
class Monitor{
    public:
        /*! @brief Class contructor.
        */
        Monitor();

        /*! @brief Function to connect the audio client to the audio server
         * @param char* - the client name
         * @return void
        */
        void connect(char*);

        /*! @brief Function to disconnect the jack audio client from the audio server
         * @return void
        */
        void disconnect();

        /*! @brief Function to initialize the effects (enabled/disabled) depending on the state of the session when last saved.
         * @param const bool[F_NUM_INPUTS][NUM_EFFECTS] - a table of boolean to declare the status (enabled/disabled) of each effect for each input channel
         * @return void
        */
        void initialize_effects(const bool[F_NUM_INPUTS][NUM_EFFECTS]);
        
        /*! @brief Function to change state of effect.
         * @param int - refers to the corresponding channel 
         * @param int - refers to the corresponding effect 
         * @return void
        */
        bool toggle_effect(int,int);
        
        /*! @brief Function to mute the input signal.
         * @param int - refers to the corresponding channel 
         * @return void
        */
        void mute_input(int);

        /*! @brief Function to mute the output signal.
         * @param int - refers to the corresponding output channel 
         * @return void
        */
        void mute_output(int);

        // void mute_microphone();
        // void unmute_microphone();
        // void mute_instrument();
        // void unmute_instrument();

        /*! @brief The callback function used to retrieve the input buffers.
         * @return int - zero on success, non-zero on error  
        */    
        int process();

        /*! @brief Function to set a member function with a function pointer to a non member function 
         * from another class so as to have the incomming buffers updated outside of the class scope.
         * @param std::function<void(float *[F_NUM_INPUTS],float *[F_NUM_OUTPUTS])> - the pointer to the non-member function.
         * @return void
        */    
        void set_stream_buffer(std::function<void(float *[F_NUM_INPUTS],float *[F_NUM_OUTPUTS])>);

        /*! @brief Function to change the state of the monitor options (ARM/MNTR)
         * @param bool - if it is MNTR or ARM.
         * @param int - the corresponding channel
         * @return bool - the current state of the monitor-button.
        */  
        bool toggle_states(bool, int);

        /*! @brief Function to set the state of the monitor options (ARM/MNTR) with a specific value. Used for initialization.
         * @param bool - if it is MNTR or ARM.
         * @param int - the corresponding channel
         * @param bool - the value to be set.
         * @return void
        */  
        void update_states(bool,int,bool);

         /*! @brief Function to get the current state of the monitor options (ARM/MNTR).
         * @param bool[F_NUM_INPUTS] - the values to be set.
         * @param bool - if it is MNTR or ARM.
         * @return void
        */  
        void get_states(bool[F_NUM_INPUTS], bool);

    private:
        Handshake hs;
        Effects effects;
        float *input_buffers[F_NUM_INPUTS];
        float *output_buffers[F_NUM_OUTPUTS];
        std::function<void(float *[F_NUM_INPUTS],float *[F_NUM_OUTPUTS])> stream_buffer;
        std::atomic<bool> input_enabled[2][F_NUM_INPUTS]; //[arm/mntr][F_NUM_INPUTS]
};

#endif