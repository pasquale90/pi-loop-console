#ifndef GPIO_H
#define GPIO_H

// #warning "GPIO included" // include warning

// #include <atomic>
#include "gpio/buttons.h"
#include "gpio/potentiometers.h"
#include "gpio/leds.h"

#include "response.h"

// BUTTONS
#define BASE_BUTTONS 100
// POTENTIOMETERS
#define BASE_POTENTIOMETERS 200

/*! @brief Defines a GPIO based interface constisted of buttons, potentiometers and leds*/
class UI{
public:
    UI();
    void listen_user(Trigger&);
    void update_output_state(Response);
    void show();

    void initialize_display(int[8]);
    void metro_display(int);

    void turnOff();
private:

    Buttons buttons;
    Potentiometers potentiometers;
    Leds leds;
};

#endif