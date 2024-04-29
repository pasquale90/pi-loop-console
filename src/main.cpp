#include <iostream>
// #include <thread>
#include "menu.h"


// callback_process()


int main(){

    std::cout<<"Hello Piloop"<<std::endl;

    Menu& menu = Menu::getInstance();
    menu.load();
    menu.change_session();

    //calls menu.load() 

    // handle session menu
    // initialize a session
    // start console

    return 0;
}