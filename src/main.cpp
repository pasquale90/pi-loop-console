#include <iostream>
// #include <thread>
#include "menu.h"
#include "config.h"

// callback_process()

int main(){

    std::cout<<"Hello Piloop"<<std::endl;

    // Menu& menu = Menu::getInstance();

    Config& cfg = Config::getInstance();
    cfg.display();
    std::cout<<"############################################################\n\n"<<std::endl;
    cfg.open(1);
    cfg.display();

    std::cout<<"public access "<<cfg.audio_settings.bit_quantization<<std::endl;
    cfg.metronome.tempo = 100;
    cfg.save();
    
    
    //calls menu.load() 

    // handle session menu
    // initialize a session
    // start console

    return 0;
}