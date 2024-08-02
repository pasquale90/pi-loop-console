#include "gpio.h"


UI::UI(){
    wiringPiSetupGpio();
    buttons.setup(BASE_BUTTONS);
    potentiometers.setup(BASE_POTENTIOMETERS);
}

void UI::metro_display(int state){
    // std::cout<<"UI::metro_display is called with state "<<state<<std::endl;
    leds.set_metro_state(state);
}

void UI::initialize_display(DisplayInit data){
    leds.initialize_leds(data); 
}

void UI::listen_user(Trigger &trigger){

	buttons.is_pressed(trigger);

    if (trigger.control.load() == ISEMPTY) // if button is pressed, return the button press
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