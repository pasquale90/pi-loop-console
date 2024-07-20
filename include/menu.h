#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "session.h"

/*! @brief The programs menu that acts as the session manager.*/
class Menu {
    public:
        /*! @brief  Class constuctor
         */        
        Menu();

        /*! @brief Function to apply setup actions after initialization and before running.
         * @return void
         */        
        void setup();
        
        /*! @brief Prime function to run on the session thread.
         * @return void
         */        
        void load_session();

        /*! @brief Function to receive a trigger message and return a response message .
         * @param Trigger - the user input signal
         * @param Response& - the response signal (passed by reference) to reach the display methods.
         * @return void
         */        
        void notify_menu(Trigger, Response&);

        /*! @brief Function to shutdown menu
         * @return void
         */        
        void unload();

        /*! @brief Function that carries a function pointer to the session obj
         * @param std::function<void(int[9])> - the function to initialize the display object with info obtained by the session object.
         * @return void
         */        
        void set_display_initializer(std::function<void(DisplayInit)>);

        /*! @brief Function that carries a function pointer to the session obj
         * @param std::function<void(int)> - the function that is going to be called each time the metronomes ticks.
         * @return void
         */
        void set_metro_display(std::function<void(int)>);
    private:
        Session session;
        Config& cfg = Config::getInstance();

        /*! @brief Private function to edit the incoming Control messages
         * @param Control - It can be either a ChangeSession or a SaveSession trigger.Otherwise the trigger is passed to the session object 
         * @param Response - If the control sig is a changeSession or a SaveSession, the response is returned otherwise it is passed to the session object  
         * @return void
         */
        void _edit(Control,Response&);
        
        /*! @brief Private function responsible to change sessions
         * @param Control - It can be either a PREV_SESSION or a NEXT_SESSION trigger.
         * @param Response - If the control sig is a PREV_SESSION or a NEXT_SESSION, the response is returned. 
         * @return void
         */
        void _change_session(Control,Response&);
        
        /*! @brief Private function responsible to save sessions. Currently this is unsupported.
         * @param Control - It can be SAVE_SESSION trigger.
         * @param Response - If the control sig is a SAVE_SESSION, the response is returned. 
         * @return void
         */
        void _save_session();
};

#endif