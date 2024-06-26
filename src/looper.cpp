#include "looper.h"
#include <iostream>
#include <assert.h>     /* assert */
std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> Looper::update_buffer(float* input[F_NUM_INPUTS],bool armEnabled[F_NUM_INPUTS]){
// float** Looper::update_buffer(float* input[F_NUM_INPUTS],bool armEnabled[F_NUM_INPUTS]){

// fuse input signals and pass them on channel update if arm is enabled (mic arm || inst arm)
    float armed_input[BUFFER_SIZE] = {0.};
    arm_enabled.store(_fuse_input(input,armEnabled,armed_input));
    if (arm_enabled.load()){
        for (int i=0; i<num_channels;++i){
            if (record.load() == i){
                channels[i].update_rec_signal(armed_input, playback_idx.load());
            }
        }
    }

    bool playback_enabled = false;
    for (int ch=0; ch<num_channels; ++ch)    {
        if ( playback[ch].load() == true ){ //!channels[ch].isEmpty() ||
            playback_enabled = true;
            break;
        }
    }

    bool isFirstloop = (loop_length.load() == 0) && record.load() != -1;
    if (isFirstloop)
        // playback_idx+=BUFFER_SIZE;
        playback_idx.store(playback_idx.load() + BUFFER_SIZE);
    else if (playback_enabled || (loop_length.load() && record.load()!=-1) ){
        // playback_idx+=BUFFER_SIZE;
        playback_idx.store(playback_idx.load() + BUFFER_SIZE);
        if (playback_idx.load() > loop_length.load() - BUFFER_SIZE) {//+ BUFFER_SIZE
            playback_idx.store(0);
            metronome.pause();
            metronome.unpause();
        }
    }

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

void Looper::load_metronome(){
    metronome.tick_tock();
}

// logic for setting the metronome and setting the range for the playback pointer
void Looper::_initialize_looper(){

// @TODO create the metronome
    metronome.stop_timing();
    // std::cout<< "\n\n\n\n\t\t\tMetronome stop timer \n\n\n\n"<<std::endl;


    // set the loop length 
    loop_length.store(playback_idx.load());
    // update all channels to reserve such space
    for (int ch = 0; ch< num_channels; ++ch)
        if (ch == record.load())
            channels[ch].set_loop_length(true,loop_length.load());
        else 
            channels[ch].set_loop_length(false,loop_length.load());

}

void Looper::reset(){
    _set_inactive();
    for (int ch=0; ch<num_channels; ++ch){
        channels[ch].clean();
        channels[ch].reset();
    }        
    // std::cout<<"Looper is reset!"<<std::endl;
    
    
    std::cout<< "\n\n\n\n\t\t\tReset the metronome to defaults? \n\n\n\n"<<std::endl;
    metronome.clear();
    metronome.unlock();
    metronome.display();
    
}

void Looper::tap_alter_metronome(bool isHold){
    if (isHold)
        metronome.alter_tempo();
    else 
        metronome.tap_tempo();

    metronome.display();

}

void Looper::_set_inactive(){
    for (int ch=0; ch<num_channels; ++ch){
        playback[ch].store(false);
// @TODO channels.clean() instead of set_loop_length(0)
        // channels[ch].clean();
    }   
    record.store(-1);
    loop_length.store(0) ;
    playback_idx.store(0) ;
// @TODO how to reset channels????



    metronome.pause();
}

void Looper::recdub(int channel,bool isHold){
    // record-dub/stop_recording/playback/erase_previous

// check if not initialized, if first loop, if playback on other channels
    if (isHold){
        if (record.load()==channel)
            record.store(-1);
        else
            channels[channel].undub();
    }else
    {
    // check if all stoped to unpause the metronome
        bool all_stoped = true;
        for (int ch=0; ch<num_channels; ++ch){
            if (playback[ch]){
                all_stoped = false;
                break;
            }
        }

        // is playback
        if (!channels[channel].isEmpty() && !playback[channel].load())
                playback[channel].store(true);
        else if (record.load() == -1){                                             // if not writting
            if (arm_enabled.load()){                                            // if at least one input is has arm enabled, start looping. @NOTE if arm disabled for both instrunments during writting, then 0 the signal until stop writting the loop(No sound when arm is disabled). 
                record.store(channel);

                if (loop_length == 0 )
                    metronome.start_timing();
                    std::cout<< "\n\n\n\n\t\t\tMetronome start timer \n\n\n\n"<<std::endl;
            }
            else return;
        }
        else { // if already writting on a channel, then it is a save_loop + playback signal

            // check if it is the first loop;
            if (loop_length.load() == 0)
                // loop_length = playback_idx;     // initialize the looper
                _initialize_looper();
                  
            if ( record.load() == channel){ // if already writting        
                record.store(-1);
                playback[channel].store(true);
                channels[channel].add_track();
            }else if (record.load()!=channel) // otherwise check if writting others
            {
                playback[record.load()].store(true);
                channels[record.load()].add_track();
                record.store(channel);                   
            }
        }
    // unpause the metronome
        if(all_stoped)
            metronome.unpause();
            
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

    playback[channel].store(false);

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
            loop_length.store(0);
            playback_idx.store(0);
            reset();
            // for (int ch=0;ch<num_channels;++ch)
                // channels[ch].set_first_loop_mode();
                // channels[ch].reset();
        }

        if (record.load() == channel)
            record.store(-1);
    }        
    else{ // Pause the playback state for this channel (check if all paused). If writting enabled for this channel check if looper_initialized.

        if (record.load() == channel){ // if already writting    
            if (loop_length.load() == 0)             // check if it is the first loop;
                // loop_length = playback_idx; // initialize the looper
                _initialize_looper();
            record.store(-1);
            channels[channel].add_track();
        } 
        bool allPaused = true;
        for (int ch=0; ch<num_channels;++ch){
            if (playback[ch].load()) {
                allPaused = false;
                break;
            }
        }
        if (allPaused){
            playback_idx.store(0);
            metronome.pause();
        }
            
    }
}

bool Looper::save(){
// @TODO integrate AudioFile
// @TODO save metronome
}

void Looper::start_stop_all(bool isHold){
    // create an atomic variable to stop.
    // set channels to indicate at the first position of the ring buffer
    if (isHold){
        reset();
    }else
    {
        bool atLeastOneIsPlaying = false;           // find out if any channel is playing 
        for (int ch=0; ch<num_channels; ++ch){
            if (playback[ch].load()){
                atLeastOneIsPlaying = true;
                break;
            }
        }
        if (record.load()!=-1){
            recdub(record.load(),false);            // stop recording 
        }
        if (atLeastOneIsPlaying){                   // pause all playbacks
            for (int ch=0; ch<num_channels; ++ch)
                playback[ch].store(false);
            playback_idx.store(0);
            metronome.pause();
        }else{
            for (int ch=0; ch<num_channels; ++ch){  // unpause them all
                if (!channels[ch].isEmpty())
                    playback[ch].store(true);
            }
            metronome.unpause();
        }
    }
}

std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> Looper::mix(){

    std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> looper_output {0};
    for (int ch=0; ch<num_channels; ++ch){
        if ( playback[ch].load() == true ){
            if ( !channels[ch].isEmpty() ){ // || playback[ch]
                std::vector<float> chout = channels[ch].get_out_signal(playback_idx.load());
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
    bool rec1 = (record.load() == 0) ? true :false;
    bool rec2 = (record.load() == 1) ? true :false;
    bool rec3 = (record.load() == 2) ? true :false;
    std::cout<<"################## DISPLAY LOOPER STATES ##################"<<std::endl;
    std::cout<<"        CH1        |        CH2        |        CH3"<<std::endl;
    std::cout<<"rec:     "<<rec1<<"         |        "<<rec2<<"          |        "<<rec3<<std::endl;
    std::cout<<"play:    "<<playback[0].load()<<"         |        "<<playback[1].load()<<"          |        "<<playback[2].load()<<std::endl;
    std::cout<<"#tracks: "<<channels[0].get_num_tracks()<<"         |        "<<channels[1].get_num_tracks()<<"          |        "<<channels[2].get_num_tracks()<<std::endl;
/*
    // std::cout<<"rec_id:  "<<channels[0].GET_REC_ID()<<"         |        "<<channels[1].GET_REC_ID()<<"          |        "<<channels[2].GET_REC_ID()<<std::endl;
    // std::cout<<"play_id  "<<channels[0].GET_PLAY_ID()<<"         |        "<<channels[1].GET_PLAY_ID()<<"          |        "<<channels[2].GET_PLAY_ID()<<std::endl;    
    std::cout<<"loop_length = "<<loop_length.load()<<std::endl;
    std::cout<<"record = "<<record.load()<<std::endl;
    // if (loop_length)
        for (int i=0; i<num_channels;++i){
            channels[i].debug();
        }
        std::cout<<"playback id "<<playback_idx.load()<<std::endl;
*/
    std::cout<<"###########################################################"<<std::endl;
}