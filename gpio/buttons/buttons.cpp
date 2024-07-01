#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>

#include <wiringPi.h>
#include <mcp23017.h>

#define GPIO_ONOFF 25
#define GPIO_START_STOP_ALL 18
#define GPIO_TAP_TEMPO 15
#define GPIO_SAVE_JAM 19
#define GPIO_PREV_SESSION 20
#define GPIO_NEXT_SESSION 12
//#define GPIO_SAVE_SESSION 23

int buttons[16] = {};


// int BUTTONS[4] = {11,9,10,8};
int main (int argc, char *argv[]){

	wiringPiSetupGpio();
	mcp23017Setup(100, 0x20);

	for (int i = 0; i < 16; ++i){
    		pinMode(100 + i, INPUT);
		pullUpDnControl (100 + i, PUD_UP);
	}

	pinMode(GPIO_ONOFF, INPUT);
	pullUpDnControl (GPIO_ONOFF, PUD_UP);
	pinMode(GPIO_START_STOP_ALL, INPUT);
	pullUpDnControl (GPIO_START_STOP_ALL, PUD_UP);
	pinMode(GPIO_TAP_TEMPO, INPUT);
	pullUpDnControl (GPIO_TAP_TEMPO, PUD_UP);
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
			// int sig1 = digitalRead(i);
			// curr = BUTTONS[i] ;
			// if (sig){
			// 	std::cout<<curr<<" is pressed"<<std::endl;
			// }
			
			// std::cout<<sig<<" "; //"("<<sig1<<") ";
			if (!sig)
				std::cout<<i<<" ";
		}

		sig = digitalRead(GPIO_ONOFF);
		if (!sig)
			std::cout<<GPIO_ONOFF<<" ";
		// std::cout<<sig<<" ";

		// sig = digitalRead(GPIO_START_STOP_ALL);
		// std::cout<<sig<<" ";

		// sig = digitalRead(GPIO_TAP_TEMPO);
		// std::cout<<sig<<" ";

		// sig = digitalRead(GPIO_SAVE_JAM);
		// std::cout<<sig<<" ";

		// sig = digitalRead(GPIO_PREV_SESSION);
		// std::cout<<sig<<" ";

		// sig = digitalRead(GPIO_NEXT_SESSION);
		// std::cout<<sig<<" ";

		std::cout<<std::endl;

		delay(100);
	}
	return 0;
}
