#ifndef PILOOP_H
#define PILOOP_H

#include <thread>
#include "interface.h"
#include "menu.h"
#include "audioserver.h"

/*! @brief The threading manager. It starts the program.*/
class PiLoop{

    public:
        /*! @brief Class contructor.
        * Responsible for setting up the threads and some function pointers to callback functions.
        */
        PiLoop();
        
        /*! @brief Runs all the four threads.
         * @return void 
         */
        void start_console();

    private:

        AudioServer server;        
        Menu menu;
        hardwareInterface interface;
        
        std::thread uiThread;
        std::thread serverThread; 
        std::thread sessionThread; 
        std::thread displayThread;
        
        /*! @brief Function running on the ui thread.
         * It passes a member function as an argument to the listener function of the user interface class.
         * @return void 
         */
        void _listen();

        /*! @brief Function which is passed to the ui thread for receiving the input signal from the user and notifying the rest of the program.
         * @param Trigger - the control message that received from the corresponding ui method.
         * @return void 
         */
        void _notify(Trigger);

        /*! @brief Function to shut down to program.
         * @return void 
         */
        void shutdown();
};

#endif

