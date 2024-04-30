#include "config.h"

// Initialize the static member to nullptr
Config* Config::cfg_instance_ptr = nullptr;

#include <iostream>
#include <typeinfo>

Config::Config(){
    _open();
} 

Config& Config::getInstance() {
  if (cfg_instance_ptr == nullptr) {
    cfg_instance_ptr = new Config(); // Create the instance if it doesn't exist
  }
  return *cfg_instance_ptr;  // Dereference the pointer to return the instance
}

void Config::open(int curr_session){

  // @TODO save the data of the last session?

  // open json file
    Json::Value root;   // will be probably destroyed at the end of this scope
    std::ifstream config_doc(cfg_json_path, std::ifstream::binary);
    config_doc >> root;

  // get session data
    // int num_sessions = root["sessions"].size();
    auto session_data = root["sessions"][curr_session];
    std::cout<< session_data["name"] <<"  ---->  is of type"<<typeid(session_data).name()<<"\n\n\n\n"<<std::endl;
  
  // store data
    _parse_json(session_data);

  //save the current session as the lat session
    root["last_session"] = curr_session;
    std::ofstream os(cfg_json_path, std::ios::binary);
    os << root;
    os.close();
    std::cout<<std::endl; //flush
    current_session = curr_session;
}


void Config::display(){
    std::cout<<"#################### Session "<<current_session<<" Config ####################\n";
    std::cout<<"---------------------- session name : "<<currSession_name<<" ----------------------------\n";
    std::cout<<"---------------------- audio ----------------------------\n";
    std::cout<<"sampling rate            \t:\t"<<audio_settings.sample_rate<<std::endl;
    std::cout<<"quantization             \t:\t"<<audio_settings.bit_quantization<<std::endl;
    std::cout<<"buffer size              \t:\t"<<audio_settings.buffer_size<<std::endl;
    std::cout<<"--------------------- metronome ----------------------------\n";
    std::cout<<"Tempo                    \t:\t"<<metronome.tempo<<std::endl;
    std::cout<<"rythm_numerator          \t:\t"<<metronome.rythm_numerator<<std::endl;
    std::cout<<"rythm_denominator        \t:\t"<<metronome.rythm_denominator<<std::endl;
    std::cout<<"#########################################################\n\n";
}

void Config::save(){
    Json::Value root;   //   // open json file (doc : starts as "null"; will contain the root value after parsing)
    std::ifstream config_doc(cfg_json_path, std::ifstream::binary);
    config_doc >> root;
  
  // replace all data with current values
    root["sessions"][current_session]["name"] = currSession_name;
    root["sessions"][current_session]["metronome"]["tempo"] = metronome.tempo;
    root["sessions"][current_session]["metronome"]["rythm_denominator"] = metronome.rythm_denominator;
    root["sessions"][current_session]["metronome"]["rythm_numerator"] = metronome.rythm_numerator;
    root["sessions"][current_session]["audio_settings"]["buffer_size"] = audio_settings.buffer_size;
    root["sessions"][current_session]["audio_settings"]["sample_rate"] = audio_settings.sample_rate;
    root["sessions"][current_session]["audio_settings"]["bit_quantization"] = audio_settings.bit_quantization;

    std::ofstream os(cfg_json_path, std::ios::binary);
    os << root;
    os.close();
    std::cout<<std::endl; //flush
}

int Config::get_max_sessions(){
    return max_sessions;
}

int Config::get_curr_session(){
    return current_session;
}

void Config::_open(){
    Json::Value root;   //   // open json file (doc : starts as "null"; will contain the root value after parsing)
    std::ifstream config_doc(cfg_json_path, std::ifstream::binary);
    config_doc >> root;

    max_sessions = root["sessions"].size(); // initialize max_sessions

    int last_session = root["last_session"].asInt();   // retrieve last session
    current_session = last_session;
    std::cout<<"last session(==current_session) is "<<last_session<<"=="<<current_session<<std::endl;

  // get data
    auto session_data = root["sessions"][current_session];
    std::cout<< session_data["name"] <<"  ---->  is of type"<<typeid(session_data).name()<<"\n\n\n\n"<<std::endl;
    _parse_json(session_data);
}

void Config::_parse_json(Json::Value data){
    // parse data
    currSession_name = data["name"].asString();
    metronome.tempo=data["metronome"]["tempo"].asFloat();
    metronome.rythm_denominator=data["metronome"]["rythm_denominator"].asInt();
    metronome.rythm_numerator=data["metronome"]["rythm_numerator"].asInt();
    audio_settings.buffer_size=data["audio_settings"]["buffer_size"].asInt();
    audio_settings.sample_rate=data["audio_settings"]["sample_rate"].asInt();
    audio_settings.bit_quantization=data["audio_settings"]["bit_quantization"].asInt();
}




