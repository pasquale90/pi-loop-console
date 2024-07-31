#ifndef COMPUTER_H
#define COMPUTER_H

// #warning "Computer included"

#include "computer/keyboard.h"
#include "computer/screen.h"
#include "response.h"

/*! @brief Type alias of a compile time sized array
  */
using DisplayInit = int[(F_NUM_INPUTS*(2+NUM_EFFECTS))+1];


/*! @brief Defines a computer based interface constisted of a keyboard and the screen*/
class UI{
public:
    /*! @brief Class contructor.
    */
    UI();

    /*! @brief Method to pass the trigger stucture by reference to a function that gets the keyboard input messages by the user.
     *  @param Trigger& - The trigger to be set by the user using the pc-keyboard.
     * @return void
     */
    void listen_user(Trigger&);

    /*! @brief Method to update the output based on the system's response message.
     * @param Response - the program's response carried to the displayer.
     * @return void
     */
    void update_output_state(Response);

    /*! @brief Method that runs on the display thread.
     * @return void
     */
    void show();

    /*!Method to pass some initialization values to the displayer before the program starts
     * @param DisplayInit - integer data to initialize the display object.
     * @return void
     */
    void initialize_display(DisplayInit);

    /*! @brief Method to call the displayer's tick_tock function.
     * @param int - the intonation signal obtained from the metronome obj.
     * @return void
     */
    void metro_display(int);

    /*! @brief Method to shutdown UI.
     * @return void
     */
    void turnOff();
private:

    Keyboard keyboard;
    Screen screen;
};

#endif