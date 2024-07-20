#include "gpio/potentiometers.h"

Potentiometers::Potentiometers(){
	conversion_ratio = (int)(MAX_SENSOR_VALUE - MIN_SENSOR_VALUE) / (MAX_VOLUME - MIN_VOLUME);
}

void Potentiometers::setup(int base){
	// wiringPiSetupGpio();

	this->base = base;
	mcp3004Setup (base, SPI_CHAN) ; // 3004 and 3008 are the same 4/8 channels

	addresses[0] = base + ANALOG_INPUT_ADDRESS_1;
	addresses[1] = base + ANALOG_INPUT_ADDRESS_2;
	addresses[2] = base + ANALOG_INPUT_ADDRESS_3;
	
	for (int i=0; i< NUM_ROTARIES ;++i){
		volumes[i] = _get_analog_input(i);
		std::cout<<"["<<i<<"] add "<<addresses[i]<<" vol "<<volumes[i]<<" get_analog_in "<<_get_analog_input(i);
	}std::cout<<std::endl;
	
}

void Potentiometers::is_changed(Trigger &trigger){
	
	int vol = _get_analog_input(0);
	if (abs(volumes[0]-vol) > 3){
		if (volumes[0] > vol){
			trigger.control.store(23); //CH1_VOL_LOW;
			trigger.subval.store(vol);
			volumes[0] = vol;
			return;
		}

		if (volumes[0] < vol){
			trigger.control.store(24); //CH1_VOL_HIGH;
			trigger.subval.store(vol);
			volumes[0] = vol;
			return;
		}
	}

	vol = _get_analog_input(1);
	if (abs(volumes[1]-vol) > 3){
		if (volumes[1] > vol){
			trigger.control.store(25); //CH2_VOL_LOW;
			trigger.subval.store(vol);
			volumes[1] = vol;
			return;
		}

		if (volumes[1] < vol){
			trigger.control.store(26); //CH2_VOL_HIGH;
			trigger.subval.store(vol);
			volumes[1] = vol;
			return;
		}
	}


	vol = _get_analog_input(2);
	if (abs(volumes[2]-vol) > 3){
		if (volumes[2] > vol){
			trigger.control.store(27); //CH3_VOL_LOW;
			trigger.subval.store(vol);
			volumes[2] = vol;
			return;
		}

		if (volumes[2] < vol){
			trigger.control.store(28); //CH3_VOL_HIGH;
			trigger.subval.store(vol);
			volumes[2] = vol;
			return;
		}
	}	

	// if (msg.load() !=-1){
	// 	// std::cout<<"Changing pot "<<msg.load()<<" at intensity "<<tval.load()<<std::endl;
	// }else msg.store(-1);

}

int Potentiometers::_get_analog_input(int channel){

	return (analogRead(addresses[channel]) / conversion_ratio);
}

