#include "interface.h"
#define MS_SLEEP 30
#include <chrono>
#include <thread>

hardwareInterface::hardwareInterface(){
    keep_running.store(true);
}

void hardwareInterface::get_metro_display(int state){
    ui.metro_display(state);
}

void hardwareInterface::get_display_initializer(int data[(F_NUM_INPUTS*(2+NUM_EFFECTS))+1]){
    ui.initialize_display(data);
}

void hardwareInterface::listen(void (PiLoop::*_notify)(Trigger), PiLoop& pl){

    while(keep_running.load()){
        ui.listen_user(trigger);
   
        if (trigger.control != ISEMPTY){
            (pl.*_notify)(trigger);
            trigger.reset();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(MS_SLEEP));
    }

}

void hardwareInterface::display(){

    while(keep_running.load()){
        ui.show();
        std::this_thread::sleep_for(std::chrono::milliseconds(MS_SLEEP));
    }  

}

void hardwareInterface::update(Response response){
    ui.update_output_state(response);
}

void hardwareInterface::deafen(){
    keep_running.store(false);
    std::cout<<"interface::deafening"<<std::endl;
    ui.turnOff();
}