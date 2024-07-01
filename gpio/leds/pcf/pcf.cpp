#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>

#include <unistd.h>
#include <wiringPi.h>
#include <pcf8574.h>

#define ADDR 0x27
#define BASE 100

int main (int argc, char *argv[]){
        std::cout<<"Hello!"<<std::endl;

        wiringPiSetupGpio();

	pcf8574Setup(BASE, ADDR);

        for(int i =0; i<8; ++i)
		pinMode(BASE + i, OUTPUT);
		
        for(int i =0; i<8; ++i){
std::cout<<"writing led "<<i<<std::endl;
		digitalWrite(BASE + i, HIGH);
		delay(500);
		digitalWrite(BASE + i, LOW);
	}

	return 0;
}
