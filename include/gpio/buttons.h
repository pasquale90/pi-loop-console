#ifndef BUTTONS_H
#define BUTTONS_H

//@TODO I2C BUS as configuration param
// BUTTONS
#define MCP23017_NUM_BUTTONS 16
#define GPIO_MCP23017_SDA 2
#define GPIO_MCP23017_SCL 3
// 5 buttons have been set up directly on the following pins
#define GPIO_ONOFF 4
#define GPIO_SAVE_JAM 15
#define GPIO_PREV_SESSION 18
#define GPIO_NEXT_SESSION 14
//#define GPIO_SAVE_SESSION 23

#include <array>
#include <unordered_map>
#include <atomic>
#include <chrono>

#include <wiringPi.h>
#include <mcp23017.h>

#include "trigger.h"

/*! @brief Buttons as the GPIO-based input interface.*/
class Buttons{

    public:
        /*! @brief Class contructor.
         */
        Buttons();

        /*! @brief Method used as an implicit constructor to setup a buttons instance.
         * @param int - the base number for the mcp23017 IO expander
         */
        void setup(int);

        /*! @brief Function to return if a button is pressed by the user.
         * @param Trigger& - the trigger to be returned.
         * @return void
         */
        void is_pressed(Trigger&);

    private: 
        
        int base;
        int i2c_bus_addr;

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
