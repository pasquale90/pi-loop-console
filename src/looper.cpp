#include "looper.h"
#include <iostream>

std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> *Looper::update_buffer(float* input[F_NUM_INPUTS],bool armEnabled[F_NUM_INPUTS]){
// fuse input signals and pass them on channel update if arm is enabled (mic arm || inst arm)
    
    float armed_input[BUFFER_SIZE] = {0.};
    arm_enabled.store(_fuse_input(input,armEnabled,armed_input));
    if (arm_enabled){
        for (int i=0; i<num_channels;++i){
            if (record.load() == i){
                channels[i].update_rec_signal(armed_input, playback_idx.load());
            }
        }
    }

    bool playback_enabled = false;
    for (int ch=0; ch<num_channels; ++ch)    {
        if ( playback[ch].load() == true ){
            playback_enabled = true;
            break;
        }
    }

    bool isFirstloop = (loop_length.load() == 0) && record.load() != -1;
    if (isFirstloop)
        playback_idx.store(playback_idx.load() + BUFFER_SIZE);
    else if (playback_enabled || (loop_length.load() && record.load()!=-1) ){
        playback_idx.store(playback_idx.load() + BUFFER_SIZE);
        if (playback_idx.load() > loop_length.load() - BUFFER_SIZE) {
            playback_idx.store(0);
            metronome.sync();
        }
    }

    ticktock.store(metronome.tick_tock());

    return mix();
}

int Looper::get_metronome_state(){
    return ticktock.exchange(-1);
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
    loop_length.store(0) ;
}

void Looper::_initialize_looper(){
    loop_length.store(playback_idx.load()); // set the loop length 
    metronome.stop_timing(loop_length.load()); // logic for setting the metronome and setting the range for the playback pointer
}

void Looper::reset(){
    _set_inactive();
    loop_length.store(0) ;
    
    for (int ch=0; ch<num_channels; ++ch){
        channels[ch].clean();
        channels[ch].reset();
    }

    metronome.clear();
    metronome.unlock();    
    // metronome.display();
}

void Looper::tap_alter_metronome(bool isHold){
    if (isHold)
        metronome.alter_tempo();
    else 
        metronome.tap_tempo();
    // metronome.display();
}

void Looper::_set_inactive(){
    for (int ch=0; ch<num_channels; ++ch){
        playback[ch].store(false);
    }
    record.store(-1);
    playback_idx.store(0) ;
    metronome.pause();
}

void Looper::_update_response(Response &response){
    response.msg = LOOPER;
    for (int ch=0; ch<num_channels; ++ch){
        response.looper_state.playbacks[ch].store(playback[ch].load());
        if (record.load()==ch)
            response.looper_state.record[ch].store(true);
        response.looper_state.num_tracks[ch].store(channels[ch].get_num_tracks());
    }
}

void Looper::recdub(int channel,bool isHold,Response &response){
    // 4 operations 
    // record-dub/stop_recording/playback/erase_previous

    if (isHold){
        if (record.load()!=channel){                     //is undub only if not writting on this channel
            channels[channel].undub();
            response.looper_state.is_changed.store(true);

        }
        else{                                            // else it just cancels the recording. The edge cases are : 
            if (loop_length.load() == 0 ){                       // 1) it is the first loop, reset the looper to abort recording
                response.msg = LOOPER; // simply declare an empty message to be returned
                reset();
            }           
            else{                                               // 2)  if writting already, abort the loop
                int prev = record.exchange(-1);
                if(prev!=-1){
                    response.looper_state.is_changed.store(true);
                }    
            }
        }
    }else
    {
        // check if all stoped to unpause the metronome
        bool all_stoped = true;
        for (int ch=0; ch<num_channels; ++ch){
            if (playback[ch].load()){
                all_stoped = false;
                break;
            }
        }

        if (!channels[channel].isEmpty() && !playback[channel].load()){         // is playback (if the channel not empty and not playing)
                playback[channel].store(true);
                response.looper_state.is_changed.store(true);
        }
        else if (record.load() == -1){                                          // is record (if not writting)
            if (arm_enabled.load()){                                            // if at least one input is has arm enabled, start recording. @NOTE if arm disabled for both instrunments during writting, then 0 the signal until stop writting the loop(No sound when arm i>
                record.store(channel);
                response.looper_state.is_changed.store(true);
                if (loop_length.load() == 0 )                                       // if it is the first loop, start timing the metronome
                    metronome.start_timing();
            }
            else {                                                                // else if arm disabled, do nothing..    
                printf("\033[%d;%dH         to make a REC, enable an ARM state ...", 15+4, 0);
                return ;                                                             
            }
        }
        else {                                                                  // if already writting on a channel, then it is a save_loop + playback signal

            if (loop_length.load() == 0)                                            // check if it is the first loop
                _initialize_looper();                                                   // initialize the looper

            if ( record.load() == channel){                                         // if already writting (on this channel)             
                record.store(-1);
                playback[channel].store(true);
                channels[channel].add_track();
                response.looper_state.is_changed.store(true);

            }else if (record.load()!=channel)                                       // otherwise check if writting others
            {
                playback[record.load()].store(true);                                //whatever channel was in writting phase, pause writting and playback
                channels[record.load()].add_track();
                record.store(channel);
                response.looper_state.is_changed.store(true);
            }
        }

        if(all_stoped){                                                         // if all channels were stoped before response, unpause the metronome
            metronome.unpause();
        }            
    }
    if (response.looper_state.is_changed.load())
        _update_response(response);
}

void Looper::stoperase(int channel,bool isHold,Response &response){
    // responsible for stop_playback/stop_recording/erase_channel

// @TODO fix the following bugg
// <-- erasing first channel, while another channel is recording ..
// <-- loop length becomes 0, while other channel is still recording
// <-- new loop length defined. The behaviour will be probably undefined.

    playback[channel].store(false);         // whatever it is, pause the playback.
    if (!channels[channel].isEmpty()){
         // stop playback turns green led on when channel is not empty
        response.looper_state.is_changed.store(true);
    }

    if (isHold){                                        // erase tracks on channel . If all other are empty, reset the looper

        if (record.load() == channel){                      // but before..., in case that it was writting, stop writting.
            record.store(-1);
            response.looper_state.is_changed.store(true);
        }

        channels[channel].clean();                          // erase only the channel of request

        bool allEmpty = true;
        for (int ch=0; ch<num_channels;++ch){
            if (!channels[ch].isEmpty()) {
                allEmpty = false;
                break;
            }
        }
        if (allEmpty){                                      // if all channels are empty, reset the looper
            response.msg = LOOPER; // simply declare an empty message to be returned
            reset();
        }
    }        
    else{       // Pause the playback state for this channel (check if all paused). If writting is currently enabled for this channel, check if looper_initialized.

        if (record.load() == channel){              // if already writting    
            if (loop_length.load() == 0)                 // check if it is the first loop;
                _initialize_looper();
            record.store(-1);
            channels[channel].add_track();
            response.looper_state.is_changed.store(true);
        } 
        bool allPaused = true;
        for (int ch=0; ch<num_channels;++ch){
            if (playback[ch].load()) {
                allPaused = false;
                break;
            }
        }
        if (allPaused){
            _set_inactive();
        }
    }

    if (response.looper_state.is_changed.load())
        _update_response(response);
}

bool Looper::save(){
// @TODO integrate AudioFile
// @TODO save metronome
}

void Looper::start_stop_all(bool isHold,Response &response){

    if (isHold){
        response.msg = LOOPER; // simply declare an empty message to be returned
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
            return recdub(record.load(),false, response);            // stop recording
            response.looper_state.is_changed.store(true);
        }
        if (atLeastOneIsPlaying){                   // pause all playbacks
            _set_inactive();
            response.looper_state.is_changed.store(true);
        }else{
            for (int ch=0; ch<num_channels; ++ch){  // unpause them all
                if (!channels[ch].isEmpty()){
                    playback[ch].store(true);
                }
            }
            metronome.unpause();
            response.looper_state.is_changed.store(true);
        }
    }
    if (response.looper_state.is_changed.load())
    _update_response(response);
}


std::array< std::array<float, BUFFER_SIZE>, F_NUM_OUTPUTS> *Looper::mix(){
    // set loop_buffer to zero before adding channels.
    // This is the fastest way to do it. You traverse twice the loop_buffer, and once each channel per circle
    for (int speaker=0; speaker<F_NUM_OUTPUTS;++speaker)
        looper_output[speaker].fill(0);

    for (int ch=0; ch<num_channels; ++ch){
        if ( playback[ch].load() == true ){
            if ( !channels[ch].isEmpty() ){ // || playback[ch]
                std::vector<float> chout = channels[ch].get_out_signal(playback_idx.load());
                for (int i=0; i< BUFFER_SIZE; ++i){
                    for (int speaker=0; speaker<F_NUM_OUTPUTS;++speaker){
                        looper_output[speaker][i] += chout[i] * channels[ch].get_volume();
                    }
                }
            }
        }
    }
    return &looper_output;
}

void Looper::volume_change(int channel,int volume){
    channels[channel].set_volume(volume);
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
    std::cout<<"###########################################################"<<std::endl;
}