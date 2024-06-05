#include "looper.h"
#include <iostream>

std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> Looper::update_buffer(float* input[F_NUM_INPUTS],bool armEnabled[F_NUM_INPUTS]){
// float** Looper::update_buffer(float* input[F_NUM_INPUTS],bool armEnabled[F_NUM_INPUTS]){

// fuse input signals and pass them on channel update if arm is enabled (mic arm || inst arm)
    float armed_input[BUFFER_SIZE] = {0.};
    arm_enabled = _fuse_input(input,armEnabled,armed_input);
    if (arm_enabled){
        for (int i=0; i<num_channels;++i){
            if (record == i){
                channels[i].update_rec_signal(armed_input, playback_idx);
            }
        }
    }

    bool playback_enabled = false;
    for (int ch=0; ch<num_channels; ++ch)    {
        if ( playback[ch] == true ){ //!channels[ch].isEmpty() ||
            playback_enabled = true;
            break;
        }
    }

    bool isFirstloop = (loop_length == 0) && record != -1;
    if (playback_enabled || (loop_length && record!=-1) ){
        playback_idx+=BUFFER_SIZE;
        if (playback_idx > loop_length-1 ) {//+ BUFFER_SIZE
            playback_idx = 0;
        }
    }
    else if (isFirstloop)
        playback_idx+=BUFFER_SIZE;

    return mix();
}

bool Looper::_fuse_input(const float* const input[F_NUM_INPUTS],bool armEnabled[F_NUM_INPUTS],float armed_input[BUFFER_SIZE]){
    bool isArmed = false;
    for (int i = 0; i<F_NUM_INPUTS; ++i){
        if (armEnabled[i]){
            isArmed = true;
            for (int j = 0 ; j < BUFFER_SIZE; ++j )
                armed_input[j] = input[i][j] + armed_input[j]; // mix instruments
        }
    }
    return isArmed;
}

Looper::Looper(){
    channels[0].set_name("LoopCh1");
    channels[1].set_name("LoopCh2");
    channels[2].set_name("LoopCh3");
    _set_inactive();
}

// logic for setting the metronome and setting the range for the playback pointer
void Looper::_initialize_looper(){
    // set the loop length 
    loop_length = playback_idx;
    // update all channels to reserve such space
    for (int ch = 0; ch< num_channels; ++ch)
        if (ch == record)
            channels[ch].set_loop_length(true,loop_length);
        else 
            channels[ch].set_loop_length(false,loop_length);

// @TODO create the metronome
}

void Looper::reset(){
    _set_inactive();
    for (int ch=0; ch<num_channels; ++ch)
        channels[ch].clean();
    // std::cout<<"Looper is reset!"<<std::endl;
}

void Looper::_set_inactive(){
    for (int ch=0; ch<num_channels; ++ch){
        playback[ch] = false;
// @TODO channels.clean() instead of set_loop_length(0)
        // channels[ch].clean();
    }   
    record = -1;
    loop_length = 0 ;
    playback_idx = 0 ;
// @TODO how to reset channels????
}

void Looper::recdub(int channel,bool isHold){
    // record-dub/stop_recording/playback/erase_previous

// check if not initialized, if first loop, if playback on other channels
    if (isHold){
        if (record==channel)
            record = -1;
        else
            channels[channel].undub();
    }else
    {
        // is playback
        if (!channels[channel].isEmpty() && !playback[channel])
                playback[channel] = true;
        else if (record == -1){                                             // if not writting
            if (arm_enabled)                                            // if at least one input is has arm enabled, start looping. @NOTE if arm disabled for both instrunments during writting, then 0 the signal until stop writting the loop(No sound when arm is disabled). 
                record = channel;
            else return;
        }
        else { // if already writting on a channel, then it is a save_loop + playback signal

            // check if it is the first loop;
            if (loop_length == 0)
                // loop_length = playback_idx;     // initialize the looper
                _initialize_looper();
                  
            if ( record == channel){ // if already writting        
                record = -1;
                playback[channel] = true;
                channels[channel].add_track();
            }else if (record!=channel) // otherwise check if writting others
            {
                playback[record] = true;
                channels[record].add_track();
                record = channel;                   
            }
        }
    }
}
void Looper::stoperase(int channel,bool isHold){
    // responsible for stop_playback/stop_recording/erase_channel

// @TODO fix the following bugg
// c
// Looper::_initialize : is initialized with loop_length = 157440
// ################## DISPLAY LOOPER STATES ##################
//         Ch1        |        CH2        |        CH3
// rec:     0         |        1         |        0
// play:    1         |        0         |        0
// #tracks: 1         |        0         |        0
// loop_length = 157440
// ###########################################################
// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                        <-- erasing first channel, while channel 2 is recording
// ################## DISPLAY LOOPER STATES ##################
//         Ch1        |        CH2        |        CH3
// rec:     0         |        1         |        0
// play:    0         |        0         |        0
// #tracks: 0         |        0         |        0
// loop_length = 0                                                   <-- loop length becomes 0, while channel 2 is still recording
// ###########################################################
// vLooper::_initialize : is initialized with loop_length = 71680
// ################## DISPLAY LOOPER STATES ##################
//         Ch1        |        CH2        |        CH3
// rec:     0         |        0         |        0
// play:    0         |        0         |        0
// #tracks: 0         |        1         |        0
// loop_length = 71680                                               <-- new loop length defined. The behaviour will be probably undefined.
// ###########################################################

    playback[channel] = false;

    if (isHold){ // erase tracks on channel . If all other are empty, reset the looper
        channels[channel].clean();
        bool allEmpty = true;
        for (int ch=0; ch<num_channels;++ch){
            if (!channels[ch].isEmpty()) {
                allEmpty = false;
                break;
            }
        }
        if (allEmpty){
            loop_length = 0;
            playback_idx = 0;
        }

        if (record == channel)
            record = -1;
    }        
    else{ // Pause the playback state for this channel (check if all paused). If writting enabled for this channel check if looper_initialized.

        if (record == channel){ // if already writting    
            if (loop_length == 0)             // check if it is the first loop;
                // loop_length = playback_idx; // initialize the looper
                _initialize_looper();
            record = -1;
            channels[channel].add_track();
        } 

        bool allPaused = true;
        for (int ch=0; ch<num_channels;++ch){
            if (playback[ch]) {
                allPaused = false;
                break;
            }
        }
        if (allPaused)
            playback_idx = 0;

    }
}

bool Looper::save(){
// @TODO integrate AudioFile
}

void Looper::start_stop_all(bool isHold){
    // create an atomic variable to stop.
    // set channels to indicate at the first position of the ring buffer
    if (isHold){
        reset();
    }else
    {
        bool atLeastOneIsPlaying = false;     
        for (int ch=0; ch<num_channels; ++ch){
            if (playback[ch]){
                atLeastOneIsPlaying = true;
                break;
            }
        }
        if (record!=-1){
            recdub(record,false);
        }
        if (atLeastOneIsPlaying){
            for (int ch=0; ch<num_channels; ++ch)
                playback[ch] = false;
            playback_idx = 0;
        }else{
            for (int ch=0; ch<num_channels; ++ch){
                if (!channels[ch].isEmpty())
                    playback[ch] = true;
            }
        }
    }
}

std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> Looper::mix(){

    std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> looper_output {0};
    for (int ch=0; ch<num_channels; ++ch){
        if ( playback[ch] == true ){
            if ( !channels[ch].isEmpty() ){ // || playback[ch]
                std::vector<float> chout = channels[ch].get_out_signal(playback_idx);
                for (int i=0; i< BUFFER_SIZE; ++i){
                    for (int speaker=0; speaker<F_NUM_OUTPUTS;++speaker){
                        looper_output[speaker][i] = looper_output[speaker][i] + chout[i] * channels[ch].get_volume();
                    }
                }
            }
        }
    }
    return looper_output;
}


void Looper::volume_up(int channel){
    channels[channel].volume_up();
}

void Looper::volume_down(int channel){
    channels[channel].volume_down();
}

void Looper::display_states(){
    bool rec1 = (record == 0) ? true :false;
    bool rec2 = (record == 1) ? true :false;
    bool rec3 = (record == 2) ? true :false;
    std::cout<<"################## DISPLAY LOOPER STATES ##################"<<std::endl;
    std::cout<<"        CH1        |        CH2        |        CH3"<<std::endl;
    std::cout<<"rec:     "<<rec1<<"         |        "<<rec2<<"          |        "<<rec3<<std::endl;
    std::cout<<"play:    "<<playback[0]<<"         |        "<<playback[1]<<"          |        "<<playback[2]<<std::endl;
    std::cout<<"#tracks: "<<channels[0].get_num_tracks()<<"         |        "<<channels[1].get_num_tracks()<<"          |        "<<channels[2].get_num_tracks()<<std::endl;
    // std::cout<<"rec_id:  "<<channels[0].GET_REC_ID()<<"         |        "<<channels[1].GET_REC_ID()<<"          |        "<<channels[2].GET_REC_ID()<<std::endl;
    // std::cout<<"play_id  "<<channels[0].GET_PLAY_ID()<<"         |        "<<channels[1].GET_PLAY_ID()<<"          |        "<<channels[2].GET_PLAY_ID()<<std::endl;    
    std::cout<<"loop_length = "<<loop_length<<std::endl;
    std::cout<<"###########################################################"<<std::endl;
}