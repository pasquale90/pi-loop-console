#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <fstream>
#include <string>
#include <json/json.h>
#include "metronome.h"

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
			struct{
					int buffer_size;
					int sample_rate;
					int bit_quantization;
			}audio_settings;
			
			// expand latter
			// struct controls{}; 
			// struct looper {};

			int get_max_sessions();
			int get_curr_session();
			
		private:
			Config();

			Config(const Config&) = delete;
			void operator=(const Config&) = delete;

			static Config* cfg_instance_ptr;
			void _open();// open only for the first time
			void _parse_json(Json::Value); //parse_json values

			int max_sessions;
			int current_session;
};

#endif