#include "mixer.h"
#include "audio_settings.h"

#include <ctime>
#include <cstring>
#include <iostream>
#include <string>

#define JAM_LENGTH SAMPLE_RATE*60*5 // the last 1 minute     //  PREPROCESSOR VALUE max_size_samples = SR* (5min*60seconds/minute)

static const std::string dataPath = "data/jams";
// @TODO move dataPath to config

Mixer::Mixer(){
    // jam_buffer.reserve(F_NUM_OUTPUTS);
    // for (int i=0; i<F_NUM_OUTPUTS;++i)
    //     jam_buffer[i].resize(JAM_LENGTH);

    jam_buffer.resize(2);
    jam_buffer[0].resize(JAM_LENGTH);
    jam_buffer[1].resize(JAM_LENGTH);

    cnt.store(0);
    curr_max_size.store(0);
}


// void Mixer::update_buffer(float* input_buffers[F_NUM_INPUTS],float* output_buffers[F_NUM_OUTPUTS], float* looper_buffers[F_NUM_OUTPUTS],bool monitorIn[F_NUM_INPUTS]){
void Mixer::update_buffer(float* input_buffers[F_NUM_INPUTS],float* output_buffers[F_NUM_OUTPUTS], std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> looper_buffers,bool monitorIn[F_NUM_INPUTS]){

    for (int i=0;i<BUFFER_SIZE;++i){
        
        // for (int j = 0 ; j < F_NUM_OUTPUTS; ++j )

#if F_NUM_INPUTS == 1
        //L
           output_buffers[0][i] = input_buffers[0][i] * (int)monitorIn[0] + looper_buffers[0][i];
        //R
            output_buffers[1][i] = input_buffers[0][i] * (int)monitorIn[0] + looper_buffers[1][i];

            jam_buffer[0][cnt.load()+i] = output_buffers[0][i];
            jam_buffer[1][cnt.load()+i] = output_buffers[1][i];

#elif F_NUM_INPUTS == 2
        //L
            output_buffers[0][i] = input_buffers[0][i] * (int)monitorIn[0] + input_buffers[1][i] * (int)monitorIn[1] + looper_buffers[0][i];
        //R
            output_buffers[1][i] = input_buffers[0][i] * (int)monitorIn[0] + input_buffers[1][i] * (int)monitorIn[1] + looper_buffers[1][i];

            jam_buffer[0][cnt.load()+i] = output_buffers[0][i];
            jam_buffer[1][cnt.load()+i] = output_buffers[1][i];
#endif
    }

    cnt.store( cnt.load() + BUFFER_SIZE );
    if (cnt.load() + BUFFER_SIZE > JAM_LENGTH - 1 )
         cnt.store( 0 );
    if ( cnt.load() > curr_max_size.load() );
        curr_max_size.store( cnt.load() );
    // std::cout<<cnt.load()<<" "<<jam_buffer[0].size()<<" "<<jam_buffer[0].capacity()<<std::endl;

// @TODO - > DONE!
    // Mix all signals
        // Current monitor (2 inputs max) if monitor enabled
        // Looper playback (a mixed signal / or 3 separate buffers)

// @TODO compression
}

void Mixer::save_jam(){

    // save last 5 min output before streaming to speakers
    
    if (curr_max_size >0){

        jam_buffer[0].resize(curr_max_size.load());
        jam_buffer[1].resize(curr_max_size.load());

        bool ok = audioFile.setAudioBuffer (jam_buffer);
        audioFile.save (_get_jamsavepath() , AudioFileFormat::Wave);

        cnt.store(0);
        curr_max_size.store(0);

    }
}

std::string Mixer::_get_jamsavepath(){
    return dataPath + "/" + _get_datetime() + ".wav";
}

std::string Mixer::_parse_time_val(int value){
    int num_digits = 0;
    int temp = value;
    while (temp){   
     temp /= 10;
     ++num_digits;
    }
    if (num_digits == 1){
        return "0"+std::to_string(value);
    }else if (num_digits == 2){
        return std::to_string(value);
    }else if (num_digits > 2){
        return std::to_string(value).substr (num_digits-2,2);
    }
}

std::string Mixer::_get_datetime(){ 
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    
    return _parse_time_val(now->tm_year + 1900)
         + _parse_time_val (now->tm_mon + 1)
         +  _parse_time_val(now->tm_mday)  + '_' 
         +  _parse_time_val(now->tm_hour)
         +  _parse_time_val(now->tm_min)
         +  _parse_time_val(now->tm_sec);
}

// void Mixer::turnOff(){
// }