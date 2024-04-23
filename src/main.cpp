#include <iostream>
// #include <thread>
#include "menu.h"
#include "config.h"

// callback_process()

int main(){

    std::cout<<"Hello Piloop"<<std::endl;

    
    Menu& menu = Menu::getInstance();
    
    Config& cfg = Config::getInstance();

    
    std::cout<<"menu "<<menu.eraseme<<std::endl;
    std::cout<<"config "<<cfg.eraseme<<std::endl;
    //calls menu.load()

    // handle session menu
    // initialize a session
    // start console

    return 0;
}