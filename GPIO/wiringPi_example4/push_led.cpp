#include <wiringPi.h>

int main(void){

    const int button = 22;
    const int led = 5;


    wiringPiSetup();
    pinMode(button,INPUT);
    pinMode(led,OUTPUT);


    while(1){
        if (digitalRead(button)==LOW){
            digitalWrite(led,LOW);
        }
        else if (digitalRead(button)==HIGH){
            digitalWrite(led,HIGH);
        }
        delay(500);
    }
    return 0;
}