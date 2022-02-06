#include <wiringPi.h>

static bool ON=0;

bool toggle_(){
    return ON=!ON;
}

int main(void){

    const int button = 22;
    const int led = 5;


    wiringPiSetup();
    pinMode(button,INPUT);
    pinMode(led,OUTPUT);

    while(1){
   
        if (digitalRead(button)==LOW){
            toggle_();
        }

        if(ON){
            digitalWrite(led,HIGH);
        }else digitalWrite(led,LOW);
        
        delay(500);
    }
    
    return 0;
}