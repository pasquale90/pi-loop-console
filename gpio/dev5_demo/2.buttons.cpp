#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>

#include <unistd.h>
#include <wiringPi.h>
#include <pcf8574.h>


 int wpLEDS[8] = {17,27,22,10,9,11,24,23}; //gpio
// int wpLEDS[8] = {11,13,15,19,21,23,18,16}; //physical
// int wpLEDS[8] = {0,2,3,12,13,14,5,4}; //wiring pi


int main (int argc, char *argv[]){
        std::cout<<"Hello!"<<std::endl;

        wiringPiSetupGpio();
	pcf8574Setup(100, 0x27);

	for (int i = 0; i < 8; ++i){
    		pinMode(100 + i, INPUT);
    		pinMode(wpLEDS[i], OUTPUT);
	}

        int curr;
        
        while( 1==1 ){
//                printf("LOOP %u\n", b);
                for (int i = 0; i < 8; ++i){
//                   std::cout<<digitalRead(100 + i)<<" ";
   
                   int sig = digitalRead(100 + i);
                   std::cout<<sig<<" ";

                   if (sig == 0){
			std::cout<<i <<" is pressed!"<<std::endl;
                        digitalWrite(wpLEDS[i],HIGH);
			sleep(1);
                        digitalWrite(wpLEDS[i],LOW);
                   }
		}
                std::cout<<std::endl;


//           sleep(1);
       }

/*
if (!sig){

std::cout<<curr<<" is pressed"<<std::endl;

digitalWrite(wpLEDS[curr],HIGH);
delay(200);
//digitalWrite(curr,1);

digitalWrite(wpLEDS[curr],LOW);
//digitalWrite(curr,0);
delay(200);
}
                }
                std::cout<<std::endl;
                b++;
		if( b >= 8 ) b=0;
		delay(200);

	}
*/
	return 0;
}
