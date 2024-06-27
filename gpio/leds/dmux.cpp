#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>

#include <unistd.h>
#include <wiringPi.h>


int dmux[4] = {26,6,5,16}; //gpio
int Le = 21;
int e = 7;
//int dmux[4] = {13,11,19,15}; //board/physical
//int Le = 18;
//int dmux[4] = {2,0,12,3}; //wiring pi
//int Le = 5;


// int matrix[16][4] ={{0,0,0,0},{1,0,0,0},{0,1,0,0},{1,1,0,0},{0,0,1,0},{1,0,1,0},{0,1,1,0},{1,1,1,0},{0,0,0,1},{1,0,0,1},{0,1,0,1},{1,1,0,1},{0,0,1,1},{1,0,1,1},{0,1,1,1},{1,1,1,1}};

/*
14{1,0,1,1},
13{0,0,1,1},
16{1,1,1,1},
15{0,1,1,1},
10{1,0,0,1},
9{0,0,0,1},
11{0,1,0,1},
12{1,1,0,1},
8{1,1,1,0},
7{0,1,1,0},
6{1,0,1,0},
5{0,0,1,0},
4{1,1,0,0},
2{1,0,0,0},
3{0,1,0,0},
1{0,0,0,0}};
*/
int matrix[16][4] ={{1,0,1,1},{0,0,1,1},{1,1,1,1},{0,1,1,1},{1,0,0,1},{0,0,0,1},{0,1,0,1},{1,1,0,1},{1,1,1,0},{0,1,1,0},{1,0,1,0},{0,0,1,0},{1,1,0,0},{1,0,0,0},{0,1,0,0},{0,0,0,0}};


int DirectLeds[5] = {14,24,22,27,17};

int main (int argc, char *argv[]){
        std::cout<<"Hello!"<<std::endl;

        wiringPiSetupGpio();

	for (int i = 0; i < 4; ++i){
    		pinMode(dmux[i], OUTPUT);
                digitalWrite(dmux[i],LOW);
	}
	for (int i = 0; i < 5; ++i){
    		pinMode(DirectLeds[i], OUTPUT);
                digitalWrite(DirectLeds[i],LOW);
	}


        pinMode(Le, OUTPUT);
        pinMode(e, OUTPUT);
       digitalWrite(Le, HIGH);
       digitalWrite(e, LOW);


        for (int j = 0; j < 16; ++j){
                for (int i = 0; i < 4; ++i){
			if (matrix[j][i] == 0)
                            digitalWrite(dmux[i],LOW);
			else digitalWrite(dmux[i],HIGH);

		}
	delay(500);
       }
       digitalWrite(e, HIGH);

	// direct leds
	for (int i=0; i<5; ++i){
            digitalWrite(DirectLeds[i],HIGH);
            delay(500);
           digitalWrite(DirectLeds[i],LOW);
       }


	return 0;
}
