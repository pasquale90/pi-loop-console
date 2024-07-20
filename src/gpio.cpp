#include "gpio.h"


UI::UI(){
    wiringPiSetupGpio();
    buttons.setup(BASE_BUTTONS);
    potentiometers.setup(BASE_POTENTIOMETERS);
}

void UI::metro_display(int state){
    // std::cout<<"UI::metro_display is called with state "<<state<<std::endl;
    leds.tick_tock(state);
}

void UI::initialize_display(int data[9]){
//require fix::for leds you should not have 9 but 8. the 9th int is curr_session can t be displayed using the led interface
    // leds.initialize_leds(data); 
}

void UI::listen_user(Trigger &trigger){

	buttons.is_pressed(trigger);

    if (trigger.control.load() == EMPTY) // if button is pressed, return the button press
        potentiometers.is_changed(trigger);

}

void UI::show(){

    leds.display();    

}

void UI::update_output_state(Response response){
    leds.perform_operation(response);
}

void UI::turnOff(){
    leds.turnOff();
}