#include "config.h"
#include "audio_settings.h"

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
    // auto session_data = root["sessions"][curr_session];
    // std::cout<< session_data["name"] <<"  ---->  is of type"<<typeid(session_data).name()<<"\n\n\n\n"<<std::endl;

  // store data
    _parse_session(root["sessions"][curr_session]);
    // _parse_audio_settings(root["audio_settings"]);

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
    std::cout<<"device ID                \t:\t"<<DEVICE_ID<<std::endl;
    std::cout<<"audio device info        \t:\t"<<DEV_INFO<<std::endl;
    std::cout<<"sampling rate            \t:\t"<<SAMPLE_RATE<<std::endl;
    std::cout<<"quantization             \t:\t"<<BIT_QUANTIZATION<<std::endl;
    std::cout<<"buffer size              \t:\t"<<BUFFER_SIZE<<std::endl;
    std::cout<<"is sterero               \t:\t"<<STEREO_OUT<<std::endl;
    std::cout<<"has sub out              \t:\t"<<SUB_OUT<<std::endl;
    std::cout<<"phones                   \t:\t"<<PHONES<<std::endl;
    std::cout<<"has mic input            \t:\t"<<MIC<<std::endl;
    std::cout<<"has inst input           \t:\t"<<INST<<std::endl;// std::cout<<"device info              \t:\t"<<device_settings.device_info<<std::endl;
    // std::cout<<"audio device             \t:\t"<<device_settings.audio_device<<std::endl;
    // std::cout<<"sampling rate            \t:\t"<<audio_settings.sample_rate<<std::endl;
    // std::cout<<"quantization             \t:\t"<<audio_settings.bit_quantization<<std::endl;
    // std::cout<<"buffer size              \t:\t"<<audio_settings.buffer_size<<std::endl;
    // std::cout<<"is sterero               \t:\t"<<device_settings.stereoOut<<std::endl;
    // std::cout<<"has sub out              \t:\t"<<device_settings.subOut<<std::endl;
    // std::cout<<"phones                   \t:\t"<<device_settings.phones<<std::endl;
    // std::cout<<"has mic input            \t:\t"<<device_settings.micIn<<std::endl;
    // std::cout<<"has inst input           \t:\t"<<device_settings.instIn<<std::endl;
    std::cout<<"--------------------- metronome ----------------------------\n";
    std::cout<<"Tempo                    \t:\t"<<metronome.tempo<<std::endl;
    std::cout<<"rythm_numerator          \t:\t"<<metronome.rythm_numerator<<std::endl;
    std::cout<<"rythm_denominator        \t:\t"<<metronome.rythm_denominator<<std::endl;
    std::cout<<"--------------------- button's state ----------------------------\n";
    std::cout<<"IN1_ARM                  \t:\t"<<button_states[IN1_ARM]<<std::endl;
    std::cout<<"IN1_MNTR                 \t:\t"<<button_states[IN1_MNTR]<<std::endl;
    std::cout<<"IN2_ARM                  \t:\t"<<button_states[IN2_ARM]<<std::endl;
    std::cout<<"IN2_MNTR                 \t:\t"<<button_states[IN2_MNTR]<<std::endl;
    std::cout<<"IN1_EFF1                 \t:\t"<<button_states[IN1_EFF1]<<std::endl;
    std::cout<<"IN1_EFF2                 \t:\t"<<button_states[IN1_EFF2]<<std::endl;
    std::cout<<"IN1_EFF3                 \t:\t"<<button_states[IN1_EFF3]<<std::endl;
    std::cout<<"IN2_EFF1                 \t:\t"<<button_states[IN2_EFF1]<<std::endl;
    std::cout<<"IN2_EFF2                 \t:\t"<<button_states[IN2_EFF2]<<std::endl;
    std::cout<<"IN2_EFF3                 \t:\t"<<button_states[IN2_EFF3]<<std::endl;
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
    // root["sessions"][current_session]["audio_settings"]["buffer_size"] = audio_settings.buffer_size;
    // root["sessions"][current_session]["audio_settings"]["sample_rate"] = audio_settings.sample_rate;
    // root["sessions"][current_session]["audio_settings"]["bit_quantization"] = audio_settings.bit_quantization;

// @TODO
// save current button state
// save any type of information related to the storage of audio data
    root["sessions"][current_session]["UI_button_states"]["IN1_ARM"]=button_states[IN1_ARM];
    root["sessions"][current_session]["UI_button_states"]["IN1_MNTR"]=button_states[IN1_MNTR];
    root["sessions"][current_session]["UI_button_states"]["IN2_ARM"]=button_states[IN2_ARM];
    root["sessions"][current_session]["UI_button_states"]["IN2_MNTR"]=button_states[IN2_MNTR];
    root["sessions"][current_session]["UI_button_states"]["IN1_EFF1"]=button_states[IN1_EFF1];
    root["sessions"][current_session]["UI_button_states"]["IN1_EFF2"]=button_states[IN1_EFF2];
    root["sessions"][current_session]["UI_button_states"]["IN1_EFF3"]=button_states[IN1_EFF3];
    root["sessions"][current_session]["UI_button_states"]["IN2_EFF1"]=button_states[IN2_EFF1];
    root["sessions"][current_session]["UI_button_states"]["IN2_EFF2"]=button_states[IN2_EFF2];
    root["sessions"][current_session]["UI_button_states"]["IN2_EFF3"]=button_states[IN2_EFF3];    

    std::ofstream os(cfg_json_path, std::ios::binary);
    os << root;
    os.close();
    std::cout<<std::endl; //flush
}

void Config::reset(){
    // keep some values (i.e. current_session name)
    std::string curr_name = currSession_name;
    // ...

    // open json file
    Json::Value root;   // will be probably destroyed at the end of this scope
    std::ifstream config_doc(cfg_json_path, std::ifstream::binary);
    config_doc >> root;

  // get the template session data
    auto session_data = root["sessions"][0];
  
  // store data
    _parse_session(session_data);
    currSession_name = curr_name;
    
    save();
}

bool Config::get_button_state(const Control& ctl){
  // std::cout<<"ctl "<<ctl<<std::endl;
  // std::cout<<"button_states "<<button_states[ctl]<<std::endl;
  return button_states[ctl];
}
void Config::toggle_button_state(const Control& ctl){
  button_states[ctl]=!button_states[ctl];
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

    max_sessions = root["sessions"].size()-1; // initialize max_sessions

    int last_session = root["last_session"].asInt();   // retrieve last session
    current_session = last_session;
    std::cout<<"last session(==current_session) is "<<last_session<<"=="<<current_session<<std::endl;

  // get data
    // auto session_data = root["sessions"][current_session];
    // std::cout<< session_data["name"] <<"  ---->  is of type"<<typeid(session_data).name()<<"\n\n\n\n"<<std::endl;

    // std::cout<< root["audio_settings"]<<"  ---->  is of type"<<typeid(root["audio_settings"]).name()<<"\n\n\n\n"<<std::endl;

    _parse_session(root["sessions"][current_session]);
    // _parse_audio_settings(root["audio_settings"]);

    jam_savepath = root["jam_savepath"].asString();
    session_savepath = root["session_savepath"].asString();

}

void Config::_parse_session(Json::Value data){
    // parse data
    currSession_name = data["name"].asString();
    metronome.tempo=data["metronome"]["tempo"].asFloat();
    metronome.rythm_denominator=data["metronome"]["rythm_denominator"].asInt();
    metronome.rythm_numerator=data["metronome"]["rythm_numerator"].asInt();
    // audio_settings.buffer_size=data["audio_settings"]["buffer_size"].asInt();
    // audio_settings.sample_rate=data["audio_settings"]["sample_rate"].asInt();
    // audio_settings.bit_quantization=data["audio_settings"]["bit_quantization"].asInt();

    button_states[IN1_ARM] = data["UI_button_states"]["IN1_ARM"].asBool();
    button_states[IN1_MNTR] = data["UI_button_states"]["IN1_MNTR"].asBool();
    button_states[IN2_ARM] = data["UI_button_states"]["IN2_ARM"].asBool();
    button_states[IN2_MNTR] = data["UI_button_states"]["IN2_MNTR"].asBool();
    button_states[IN1_EFF1] = data["UI_button_states"]["IN1_EFF1"].asBool();
    button_states[IN1_EFF2] = data["UI_button_states"]["IN1_EFF2"].asBool();
    button_states[IN1_EFF3] = data["UI_button_states"]["IN1_ARM"].asBool();
    button_states[IN2_EFF1] = data["UI_button_states"]["IN2_EFF1"].asBool();
    button_states[IN2_EFF2] = data["UI_button_states"]["IN2_EFF2"].asBool();
    button_states[IN2_EFF3] = data["UI_button_states"]["IN2_ARM"].asBool();

}

// void Config::_parse_audio_settings(Json::Value data){
//     // device_settings.audio_device=data["selected_audio_device"].asString();
//     audio_settings.buffer_size=data["buffer_size"].asInt();
//     audio_settings.sample_rate=data["sample_rate"].asInt();
//     audio_settings.bit_quantization=data["bit_quantization"].asInt();
//     _parse_audio_device(data["audio_devices"][device_settings.audio_device]);
// }

// void Config::_parse_audio_device(Json::Value data){
//     // parse data
//     device_settings.device_info=data["device_info"].asString();
//     device_settings.stereoOut=data["stereoOut"].asBool();
// 		device_settings.subOut=data["subOut"].asBool();
// 		device_settings.phones=data["phones"].asBool();
// 		device_settings.micIn=data["micIn"].asBool();
// 		device_settings.instIn=data["instIn"].asBool();
// }