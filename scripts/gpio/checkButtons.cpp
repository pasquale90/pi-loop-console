#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>

#include <wiringPi.h>
#include <mcp23017.h>

#define GPIO_ONOFF 4
#define GPIO_SAVE_JAM 15
#define GPIO_PREV_SESSION 18
#define GPIO_NEXT_SESSION 14
//#define GPIO_SAVE_SESSION 23

int buttons[16] = {};

int main (){

	wiringPiSetupGpio();
	mcp23017Setup(100, 0x20);

	for (int i = 0; i < 16; ++i){
    		pinMode(100 + i, INPUT);
		pullUpDnControl (100 + i, PUD_UP);
	}

	pinMode(GPIO_ONOFF, INPUT);
	pullUpDnControl (GPIO_ONOFF, PUD_UP);
	pinMode(GPIO_SAVE_JAM, INPUT);
	pullUpDnControl (GPIO_SAVE_JAM, PUD_UP);
	pinMode(GPIO_PREV_SESSION, INPUT);
	pullUpDnControl (GPIO_PREV_SESSION, PUD_UP);
	pinMode(GPIO_NEXT_SESSION, INPUT);
	pullUpDnControl (GPIO_NEXT_SESSION, PUD_UP);

	int sig;
	while( true ){
		for (int i = 0; i < 16; ++i){
			sig = digitalRead(100 + i);
			if (!sig)
				std::cout<<i<<" ";
		}

		sig = digitalRead(GPIO_ONOFF);
		if (!sig)
			std::cout<<GPIO_ONOFF<<" ";

		sig = digitalRead(GPIO_SAVE_JAM);
		if (!sig)
			std::cout<<GPIO_SAVE_JAM<<" ";

		 sig = digitalRead(GPIO_PREV_SESSION);
		if (!sig)
			std::cout<<GPIO_PREV_SESSION<<" ";

		sig = digitalRead(GPIO_NEXT_SESSION);
		if (!sig)
			std::cout<<GPIO_NEXT_SESSION<<" ";

		std::cout<<std::endl;

		delay(100);
	}
	return 0;
}
