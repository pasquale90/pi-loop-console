#ifndef POTENTIOMETERS_H
#define POTENTIOMETERS_H
#define SPI_CHAN 0
#define NUM_ROTARIES 3
#define ANALOG_INPUT_ADDRESS_1 0
#define ANALOG_INPUT_ADDRESS_2 1
#define ANALOG_INPUT_ADDRESS_3 2

#define MIN_SENSOR_VALUE 0
#define MAX_SENSOR_VALUE 1000
#define MIN_VOLUME 0
#define MAX_VOLUME 100
#define CONVERTION_RATIO 2

#include <iostream>
#include <wiringPi.h>
#include <mcp3004.h>
#include "trigger.h"

/*

139 120 205
137 126 208
133 126 192

*/

/*! @brief Potentiometers to adjust the looper-channel's volume.*/
class Potentiometers{
	public:
		/*! @brief Class contructor.
         */
		Potentiometers();

		/*! @brief Method used as an implicit constructor to setup the instance of the class.
         * @param int - the base number for the mcp3008 ADC
         */
		void setup(int);
		
		/*! @brief Function to notify a knob is been rotated or not.
         * @param Trigger& - the trigger to be returned.
         * @return void
         */
		void is_changed(Trigger&);

	private:
		int base;
		int addresses[NUM_ROTARIES];
		unsigned int volumes[NUM_ROTARIES];
		int conversion_ratio;
		int _get_analog_input(int);
};


#endif