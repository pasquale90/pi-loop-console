#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>

#include <unistd.h>
#include <wiringPi.h>


int dmux[4] = {26,6,5,16}; //gpio
int Le = 21;
//int e = 18;
//int dmux[4] = {13,11,19,15}; //board/physical
//int Le = 18;
//int dmux[4] = {2,0,12,3}; //wiring pi
//int Le = 5;


int main (int argc, char *argv[]){
        std::cout<<"Hello!"<<std::endl;

        wiringPiSetupGpio();

	for (int i = 0; i < 4; ++i){
    		pinMode(dmux[i], OUTPUT);
                digitalWrite(dmux[i],LOW);
	}
        pinMode(Le, OUTPUT);
  //      pinMode(e, OUTPUT);
       digitalWrite(Le, HIGH);
    //   digitalWrite(e, LOW);

        while( 1==1 ){

                for (int i = 0; i < 4; ++i){


                        digitalWrite(dmux[i],HIGH);

std::cout<<i<<" is HIGH"<<std::endl;
			delay(500);
                        digitalWrite(dmux[i],LOW);
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
