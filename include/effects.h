#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED

#include <iostream>
#include <cmath>
#include <atomic>
#include "audio_settings.h"

/*! @brief The effects module. Currently unsupported.*/
class Effects{
    public:
        /*! @brief Class contructor.
         */
        Effects();
        
        /*! @brief Implicit initialization of all effects for each channel, depending on the state of the current session.
         * @param const bool[F_NUM_INPUTS][NUM_EFFECTS] - a boolean table containing the states of the effects (enabled/disabled) for each input channel.
         * @return void
         */
        void initialize_effects(const bool[F_NUM_INPUTS][NUM_EFFECTS]);

        /*! @brief Enables/Disables the an effect.
         * @param int - the corresponding channel
         * @param int - the corresponding effect
         * @return bool - the current state of the corresponding effect after processing.
         */
        bool toggle_effect(int,int);

        /*! @brief Function to apply all the enabled effects to the input signal altogether.
         * Currently unsupported.
         * @param float*[F_NUM_INPUTS] - the input signal
         * @return void
         */
        void apply(float*[F_NUM_INPUTS]);
        
    private:

        // Config& cfg = Config::getInstance();
        
        std::atomic<bool> effects_enabled[F_NUM_INPUTS][NUM_EFFECTS];

        void _apply_whiteNoise(float*);
        void _apply_effect2(float*);
        void _apply_effect3(float*);

};  
#endif