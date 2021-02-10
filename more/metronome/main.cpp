///*
#include <iostream>
#include "metronome.h"

int main(){

    metronome m;
    metronome* p=&m;


    std::cout<<"Press \'t\' to set the metronome and \'E\' to exit.."<<std::endl;
    char input;
    std::cin>>input;
    while(input!='E'){
        if (input=='r'){
            p->start_timing();
        }
        if (input=='s'){
            p->stop_timing();
            p->set_tempo();
            std::cout<<"Tempo value is set to:"<<p->get_tempo()<<std::endl;
        }
        std::cin>>input;
    }
    std::cout<<"Time signature is set to "<<p->get_rythm().prologue<<"/"<<"4 "<<std::endl;

    p->run();

    return 0;

}
//*/
