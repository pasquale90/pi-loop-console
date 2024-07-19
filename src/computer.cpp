#include "computer.h"


UI::UI(){

    // screen.setup();
}

void UI::metro_display(int state){
    // std::cout<<"UI::metro_display is called with state "<<state<<std::endl;
    screen.tick_tock(state);
}

void UI::initialize_display(int data[(F_NUM_INPUTS*(2+NUM_EFFECTS))+1]){
    screen.initialize_states(data);
}

void UI::listen_user(Trigger &trigger){
	keyboard.is_pressed(trigger);
}

void UI::show(){

    screen.display();    

}
#include <iostream>
void UI::update_output_state(Response response){
    
    // if (response.looper_state.is_changed){

    //     std::cout<<"\n\nComputer Received -> response.looper_state.is_changed.load()"<<std::endl;
    //     for (int ch=0; ch<3; ++ch){
    //         std::cout<< "Channel ["<<ch<<"]"<<std::endl;
    //         std::cout<<"\t\t play :"<<response.looper_state.playbacks[ch].load()<<std::endl;
    //         std::cout<<"\t\t rec :"<<response.looper_state.record[ch].load()<<std::endl;
    //         std::cout<<"\t\t num_tracks :"<<response.looper_state.has_tracks[ch].load()<<std::endl;
    //     }
    // }
    
    screen.perform_operation(response);

}

void UI::turnOff(){
    screen.turnOff();
}