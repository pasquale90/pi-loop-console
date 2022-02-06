#include <wiringPi.h>


int main(void){
    const int led=5;

    wiringPiSetup();

    pinMode(led,OUTPUT);

    while(1){
        digitalWrite(led,HIGH);
        delay(500);
        digitalWrite(led,LOW);
        delay(500);
    }

    return 0;

}