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

		// public attributes. No need for getters/setters
			std::string currSession_name;
			Metronome metronome;
			struct
			{
					int buffer_size;
					int sample_rate;
					int bit_quantization;
			}audio_settings;
			
			// expand latter
			// struct controls{}; 
			// struct looper {}; 

		private:
			Config();

			Config(const Config&) = delete;
			void operator=(const Config&) = delete;

			static Config* cfg_instance_ptr;
			void _open();
			void _parse_json(Json::Value);
			int current_session;
};

#endif