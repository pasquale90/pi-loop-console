#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <iostream>

#ifdef RPI
    #include "gpio.h"
#elif PC
    #include "computer.h"
#endif

using DisplayInit = int[(F_NUM_INPUTS*(2+NUM_EFFECTS))+1];

class PiLoop;
struct Response;

/*! @brief The generic interface of the pi-loop. It has an input and an output interface.*/
class hardwareInterface{

    public:
        /*! @brief Class contructor.
         */
        hardwareInterface();

        /*! @brief Method to call the metronome display function triggered by the Metronome
         * @param int - the metronome intonation signal.
         * @return void
         */
        void get_metro_display(int);

        /*! @brief Method to initialize the display for some display components (i.e. if EFF1 is enabled by default on the current session)
         * @param int[9] - integer data to initialize the display object.
         * @return void
         */
        void get_display_initializer(DisplayInit);

        /*! @brief Prime method that runs on the ui thread.
         * @param void (PiLoop::*_notify)(Trigger) - the function pointer fo the Piloop class that is responsible for receiving the trigger from the current function.
         * @param PiLoop& - reference to the Piloop object.
         * @return void
         */
        void listen(void (PiLoop::*_notify)(Trigger), PiLoop&);

        /*! @brief Prime method that runs on the display thread.
         * @return void
         */
        void display();

        /*! @brief Method to update the display thread upon user interventions
         * @param Response - post-process signal to update the display thread.
         * @return void
         */        
        void update(Response);

        /*! @brief Method to shutdown the interface
         * @return void
         */        
        void deafen();

    private:

        UI ui;
        std::atomic<bool> keep_running;
        Trigger trigger;

};


#endif