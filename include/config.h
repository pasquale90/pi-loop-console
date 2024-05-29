#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <fstream>
#include <string>
#include <unordered_map>
#include <json/json.h>
#include "metronome.h"
#include "controls.h"

// #ifdef K6
//     static const std::string selected_audio_device = "K6";
// #elif CODEC
//     static const std::string selected_audio_device = "CODEC";
// #elif PCH
// 	static const std::string selected_audio_device = "PCH";
// #endif

// const std::string cfg_json_path = "_configuration.json" ;
const std::string cfg_json_path = "_configuration.json" ;

class Config {
		public:
			static Config& getInstance();

			void open(int);
			void save();
			void display();
			void reset();

		// public attributes. No need for getters/setters
			std::string currSession_name;
			Metronome metronome;
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
			 
			bool get_button_state(const Control&);
			void toggle_button_state(const Control&);

			int get_max_sessions();
			int get_curr_session();
			
		private:
			Config();

			Config(const Config&) = delete;
			void operator=(const Config&) = delete;

			static Config* cfg_instance_ptr;
			void _open();// open only for the first time
			void _parse_session(Json::Value); //parse_json values
			// void _parse_audio_settings(Json::Value); //parse_json values
			// void _parse_audio_device(Json::Value); //parse_json values
			int max_sessions;
			int current_session;
};

#endif