#ifndef GPIO_H
#define GPIO_H

// BUTTONS
#define BASE 100
#define I2CBUS 0x20
#define MCP23017_NUM_BUTTONS 16
#define GPIO_MCP23017_SDA 2
#define GPIO_MCP23017_SCL 3
// 5 buttons have been set up directly on the following pins
#define GPIO_ONOFF 4
#define GPIO_SAVE_JAM 15
#define GPIO_PREV_SESSION 18
#define GPIO_NEXT_SESSION 14
//#define GPIO_SAVE_SESSION 23

#warning "GPIO-buttons included" // include warning

#include <array>
#include <unordered_map>
#include <atomic>
#include <chrono>

#include <wiringPi.h>
#include <mcp23017.h>

#include "controls.h"

class UI{

    public:
        static UI& getInstance();  // Singleton

        void listen_user(std::atomic<bool>&, std::atomic<int>&);

    private: 

        // Singleton        
        UI();
        UI(const UI&) = delete;
        void operator=(const UI&) = delete;
        static UI* ui_instance_ptr;
        
        void _initialize();
        void _setupControlMapping();
        void _initialize_timings();

        std::unordered_map<int,Control> control_mapping;
        std::unordered_map<Control,float> time_pressed;
        
        std::array<int,4> direct_buttons_helper = {GPIO_ONOFF, GPIO_SAVE_JAM, GPIO_PREV_SESSION, GPIO_NEXT_SESSION }; //GPIO_SAVE_JAM

        std::chrono::steady_clock::time_point present,past;
        int time_elapsed;

        void _helper(int, int&, bool&);
};

#endif
