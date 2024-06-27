#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>

#include <unistd.h>
#include <wiringPi.h>
#include <pcf8574.h>


//int wpLEDS[8] = {21,20,16,26,19,13,6,5};
int wpLEDS[8] = {17,27,22,10,9,11,24,23};

int KNOB = 7;

int main (int argc, char *argv[]){

        wiringPiSetupGpio();
	pcf8574Setup(100, 0x27);

	for (int i = 0; i < 8; ++i){
    		pinMode(100 + i, INPUT);
    		pinMode(wpLEDS[i], OUTPUT);
	}

  int a2dVal;
  float a2dVol;
 float Vref = 3.3;





	// KNOB 
	pinMode(KNOB,INPUT);


        int b=0;
int curr;
        while( 1==1 ){
//                printf("LOOP %u\n", b);
                for (int i = 0; i < 8; ++i){
 //                   std::cout<<digitalRead(100 + i)<<" ";
                    int sig = digitalRead(100 + i);
curr = i ;


//std::cout<<analogRead(KNOB)<<std::endl;
a2dVal = analogRead(KNOB);
a2dVol = a2dVal * Vref / 255;
printf("a2dVal=%d\n",a2dVal);
printf("a2dVol=%f[V]\n",a2dVol);
    sleep(1);


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

	return 0;
}
