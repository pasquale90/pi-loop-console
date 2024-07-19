#ifndef SESSION_H_INCLUDED
#define SESSION_H_INCLUDED

#include "monitor.h"
#include "looper.h"
#include "mixer.h"
#include "config.h"

struct Response;

/*! @brief Defines a musical session. It is consisted of a Monitor, a Looper and a Mixer.*/
class Session{
    public:
        /*! @brief Class contructor.
         */
        Session(); 
        
        /*! @brief Setup whatever needed for implicit initiatialization of the imported classes 
         * @return void
        */
        void setup();
        
        /*! @brief Threaded function. This one is to have the audio client up and running.
         * @return void
        */
        void load();
        
        /*! @brief Function to save the current session. Currently it is partially unsupported (Looper session will not be saved).
         * @return void
        */
        void save();
        
        /*! @brief Function used to change session.
         * @params int - the next session.
         * @return void
         */
        void migrate(int);

        /*! @brief Function shutdown the session
         * @return void
         */
        void evacuate(); // rename to stop_running???

        /*! @brief Function used to retrieve the user's input trigger,
         * and process the message and pass it all around the program,
         * getting sure that a response will be returned for display.
         * @return void
         */
        void notify_session(Trigger, Response&);

        /*! @brief Function to save the last minutes of the current jam.
         * @return void
         */
        void save_jam();

        /*! @brief Function to set the name of the current session.
         * @param const char* - the new name
         * @return void
         */
        void set_name(const char*);

        /*! @brief Function to return the name of the current session.
         * @return const char* - the name of the current session.
         */
        const char* get_name();

        /*! @brief Function to pass the metronome-display-function-pointer.
         * @param std::function<void(int)> - the metronome display function
         * @return void
         */
        void set_metronome_display(std::function<void(int)>);
        
        /*! @brief Function that gets a function pointer to set a member function,
         * which is used to implicitly initialize the display module with default/predefined values.
         * @param std::function<void(int[9])> - the pointer to the function that takes 9 integer values to initialize the output states before use.
         * @return void
         */
        void set_disp_initializer(std::function<void(int[(F_NUM_INPUTS*(2+NUM_EFFECTS))+1])>);

    private:
        Monitor monitor;
        Looper looper;
        Mixer mixer;
        Config& cfg = Config::getInstance();
        std::atomic<bool> is_running,is_firsTime,re_initialize;
        std::function<void(int)> _notify_metronome_display;
        std::function<void(int[(F_NUM_INPUTS*(2+NUM_EFFECTS))+1])> _initialize_display;

        /*! @brief Function that receives the input signal and shares it with looper and monitor.
         * @param float *[F_NUM_INPUTS] - the input buffer
         * @param float *[F_NUM_OUTPUTS] - the output buffer
         * @return void
         */
        void _update_buffers(float *[F_NUM_INPUTS],float *[F_NUM_OUTPUTS]);

        /*! @brief Helper function to parse datetime information used when saving the current jam
         * @param int - the datetime sub info to be parsed.
         * @return std::string - the parsed information
         */    
        std::string _parse_time_val(int);

        /*! @brief Function to return the datetime information in a format useful for storing wav files.
         * @return string - the datetime information as a parsed string.
         */
        std::string _get_datetime();

        /*! @brief Function used to update the display of the metronome.
         * @return void
         */        
        void _update_metronome_display();
        
        /*! @brief helper function used within setup() function.
         * @return void
         */      
        void _setup_display();
};

#endif