#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <fstream>
#include <string>
#include <unordered_map>
#include <json/json.h>
#include "trigger.h"

const std::string cfg_json_path = "_configuration.json" ;

/*! @brief The config module responsible for saving, loading, and initializing session data.*/
class Config {
		public:
			/*! @brief Method to return a reference to the Singleton object.
         	*/
			static Config& getInstance();

			/*! @brief Reads the information stored within the configuration file for a single session.
			* @param int - the session id.
			* @return void
         	*/
			void open(int);
			
			/*! @brief Saves the information of the current session within the configuration file.
			* @return void
         	*/
			void save();
			
			/*! @brief Displays the information of the current session.
			* @return void
         	*/
			void display();

			/*! @brief Discards all the changes made during a session.
			* @return void
         	*/
			void reset();

		// public attributes. No need for getters/setters
			std::string currSession_name;
			// struct{
			// 		int buffer_size;
			// 		int sample_rate;
			// 		int bit_quantization;
			// }audio_settings;
			
			// struct{
			// 	std::string audio_device {selected_audio_device};
			// 	std::string device_info {""};
			// 	bool stereoOut {};
			// 	bool subOut {};
			// 	bool phones {};
			// 	bool micIn {};
			// 	bool instIn {};
			// }device_settings;

			// expand latter
			// struct controls{}; 
			// struct looper {};

			std::unordered_map<Control,bool> button_states;

			/*! @brief getter for button states. Used only for initialization.
			* @param const Control& - the related button of interest
			* @return bool - the state of the current button
         	*/
			bool get_button_state(const Control&);

			/*! @brief Toggles the button states. Discarded since not thread safe.
         	*/
			void toggle_button_state(const Control&);

			/*! @brief Method to read the number of session defined within the configuration file
         	* @return int - the maximum number of sessions.
			*/
			int get_max_sessions();
			
			/*! @brief Method to return the id of the current session.
         	* @return int - the id of the current session.
			*/
			int get_curr_session();

			float tempo;
			int rythm_numerator,rythm_denominator;
			std::string jam_savepath,session_savepath;
			
		private:
			Config();
			Config(const Config&) = delete;
			void operator=(const Config&) = delete;
			static Config* cfg_instance_ptr;
			
			/*! @brief Reads the information stored within the configuration file for a single session during the construction of the config object.
			* @return void
         	*/
			void _open();// open only for the first time

			/*! @brief Method to parse the data read from the json file.
			* @param Json::Value - data related to the current session.
			* @return void
         	*/
			void _parse_session(Json::Value); //parse_json values
			
			// void _parse_audio_settings(Json::Value); //parse_json values
			// void _parse_audio_device(Json::Value); //parse_json values
			int max_sessions;
			int current_session;
};

#endif