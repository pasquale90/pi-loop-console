
#define BASE 100
#define SPI_CHAN 0

#include <iostream>
#include <wiringPi.h>
#include <mcp3004.h>

int main(){
		wiringPiSetup() ;
		mcp3004Setup (BASE, SPI_CHAN) ; // 3004 and 3008 are the same 4/8 channels

		int min[3] = {1000,1000,1000};

		while(true){
				int x1=analogRead(BASE + 0 );
				int x2=analogRead(BASE + 1 );
				int x3=analogRead(BASE + 2 );
				
				std::cout<<x1<<" "<<x2<<" "<<x3<<" "<<std::endl;

		}
}
