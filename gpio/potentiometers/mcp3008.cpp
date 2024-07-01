
#define BASE 100
#define SPI_CHAN 0

/*

139 120 205
137 126 208
133 126 192

*/

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

if (x1 < min[0] )
min[0] = x1;
if (x2 < min[1] )
min[1] = x2;
if (x3 < min[2] )
min[2] = x3;

//  std::cout<<min[0]<<" "<<min[1]<<" "<<min[2]<<" "<<std::endl;

//		<<analogRead (BASE + 3)<<" "
//		<<analogRead (BASE + 4)<<" "
//		<<analogRead (BASE + 5)<<" "
//		<<analogRead (BASE + 6)<<" "<<analogRead (BASE + 7)<<std::endl;
}
}
