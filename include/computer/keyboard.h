#ifndef KEYBOARD_H
#define KEYBOARD_H

#define COUNTOF(x)  (int) ( ( sizeof(x) / sizeof((x)[0]) ) )
#define STRCASE(x)  case x: return #x

// hack test for working with events
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <err.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <unordered_map>
#include <atomic>
#include "trigger.h"

static const char *keycodes[64 * 1024] = { 0 }; // hack
static const char *eventDevice = "/dev/input/event3";

// // used only for development purposes.
// // Release version will provide buttons as an interface for user interaction

/*! @brief The keyboard as the computer-based input interface*/
class Keyboard{

    public:
        /*! @brief Class contructor.
        */
        Keyboard();
        
        /*! @brief Function to return if a keyboard input is pressed by the user.
         * @param Trigger& - the trigger to be returned.
         * @return void
         */
        void is_pressed(Trigger&);
        
    private: 

        /*! @brief Function to setup the mapping between triggers and keycodes from the keyboard.
         * @param Trigger& - the trigger to be returned.
         * @return void
         */
        void _setupKeyCodes();

        /*! @brief Function to print the type of the keyboard event.
         * @param int - the keyboard event.
         * @return const char* - the type of the event.
         */
        const char* _printableEventType(int);


        struct libevdev *dev;
        int err;
        struct input_event ev;

        std::unordered_map<Control,int> time_pressed;        
};

#endif