#ifndef SCREEN_H
#define SCREEN_H

#define MS_BLINK 25 //25 ms blink time

#include <sstream>
#include <array>
#include <unordered_map>
#include <unistd.h>
#include <utility>
#include <atomic>
#include "trigger.h"
#include "response.h"
#include "audio_settings.h"


/*! @brief The screen as the computer-based output interface.*/
class Screen{

    public:
		/*! @brief Class contructor.
         */
		Screen();

		/*! @brief Method to impicitly initialize some output display components.
		* @param int[9] - 9 values (2x2 effects, 2x2 monitor states + the current session)
		* @return void
		*/
		void initialize_states(int[(F_NUM_INPUTS*(2+NUM_EFFECTS))+1]);

		/*! @brief Threaded function running on the display thread.
		* @return void
		*/
		void display();
	
		/*! @brief Function running on the session thread, and carries the system's response to the displayer.
		 * @return void
         */
		void perform_operation(Response);
		
		/*! @brief Method to display the metronome's state.
		 * @param int - the metronome's intonation signal
         * @return void
		 */
		void tick_tock(int);		
		
		/*! @brief Method to shutdown the displayer.
		 * @return void
         */
		void turnOff();
    private: 
		
		/*! @brief Function to display the text gui.
		 * @return void
         */
        void _display_text();

		/*! @brief Function to setup the mappings between response messages and display potential states.
         * @return void
		 */
        void _setupTextMapping();
        
		std::unordered_map<Control,std::array<std::string,2>> toggle_text_mapping;
		std::array<std::array<std::string,2>,2> looper_text_mapping;
		std::unordered_map<int,std::string> metro_mapping;

		std::array<std::array<std::atomic<int>,2>,3> looper_states;
        std::unordered_map<Control,std::atomic<int>> toggle_text_states;
		std::array<std::atomic<int>,3> mixer_states;
		std::atomic<int> num_session;
		int metro_line,curr_line;

		std::string jam_save_state, piloop_state;
			
		Response response;
		
		/*! @brief Method to change the looper's current state 
		 * @return void
         */
		void _change_looper_state_simple();

		/*! @brief Method to change the jam_saving current state
		 * @return void
         */
		void _jam_save();

		/*! @brief Method to toggle the states of effects and monitor display components.
		 * @return void
         */
		void _toggle_state();

		/*! @brief Method to display the changing of the session on the screen.
		 * @return void
         */
		void _session_change();
};

#endif
