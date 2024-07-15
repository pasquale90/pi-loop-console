#include "mixer.h"
#include "audio_settings.h"

#include <iostream>

#define JAM_LENGTH SAMPLE_RATE*60*5 // the last 5 minutes     //  PREPROCESSOR VALUE max_size_samples = SR* (5min*60seconds/minute)

static const std::string dataPath = "data/jams";
// @TODO move dataPath to config

Mixer::Mixer(){

    jam_buffer.resize(2);
    jam_buffer[0].resize(JAM_LENGTH);
    jam_buffer[1].resize(JAM_LENGTH);

    audioFile.setBitDepth (BIT_QUANTIZATION);
    audioFile.setSampleRate (SAMPLE_RATE);

    cnt.store(0);
    curr_max_size.store(0);
}

void Mixer::update_buffer(float* input_buffers[F_NUM_INPUTS],float* output_buffers[F_NUM_OUTPUTS], std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> &looper_buffers,bool monitorIn[F_NUM_INPUTS]){

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
    if ( cnt.load() > curr_max_size.load() )
        curr_max_size.store( cnt.load() );
    // std::cout<<cnt.load()<<" "<<jam_buffer[0].size()<<" "<<jam_buffer[0].capacity()<<std::endl;

// @TODO - > DONE!
    // Mix all signals
        // Current monitor (2 inputs max) if monitor enabled
        // Looper playback (a mixed signal / or 3 separate buffers)

// @TODO compression
}

void Mixer::save_jam(std::string savepath){

    // save last 5 min output before streaming to speakers
    
    jam_buffer[0].resize(curr_max_size.load());
    jam_buffer[1].resize(curr_max_size.load());

    bool ok = audioFile.setAudioBuffer (jam_buffer);

    audioFile.save ( savepath , AudioFileFormat::Wave);

    std::cout<<"\nJam "<<savepath<<" is saved"<<std::endl;
    cnt.store(0);
    curr_max_size.store(0);
}