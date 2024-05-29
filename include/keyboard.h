#ifndef KEYBOARD_H
#define KEYBOARD_H

#warning "Keyboard included"

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
#include "controls.h"

// // used only for development purposes.
// // Release version will provide buttons as an interface for user interaction

class UI{

    public:
        static UI& getInstance();  // Singleton

        // forward declare listen_user() function from either keyboards(DEV) or buttons (REL)
        void listen_user(std::atomic<bool>&, std::atomic<int>&);
        // void listen_user();

    private: 

        // Singleton        
        UI();
        UI(const UI&) = delete;
        void operator=(const UI&) = delete;
        static UI* ui_instance_ptr;

        void initialize();
        struct libevdev *dev;
        int err;
        struct input_event ev;

        std::unordered_map<Control,int> time_pressed;
        
        void listen_user_keyboard(std::atomic<bool>&, std::atomic<int>&);
};

#endif