#include "channel.h"
#include <iostream>

Channel::Channel() {
    rec_id = 0;
    play_id = 0;
    num_tracks = 0; 
    for (int state = 0; state<num_states; ++state){
        states_avail[state] = 0;
    }

// @TODO apply a method to retrieve the current volume set on corresponding potentiometer
    volume = 70;
    volume_step = volumeStep;

    
    for (int state = 0; state<num_states; ++state){
        data[state].resize(MAX_LOOP_LENGTH_SAMPLES);   // resize to avoid push backs.
        data[state].shrink_to_fit();
    }
}

void Channel::reset(){

    for (int state = 0; state<num_states; ++state){
        std::fill(data[state].begin(), data[state].end(), 0);   // set to zero rather than clearing or erasing. Zeros required for dubing + do not affect size
    }
}

void Channel::set_name(const char* name){
    channel_name = name;
}

const char* Channel::get_name(){
    return channel_name;
}

void Channel::update_rec_signal(float monitor[BUFFER_SIZE],int idx){
    // https://www.gnu.org/software/libc/manual/html_node/Alloca-Example.html
    if (num_tracks == 0){ // if no track on the channel recorded yet.
        rec(monitor,idx);
    }else {
        dub(monitor,idx);
    }
}

void Channel::rec(float monitor[BUFFER_SIZE], int idx){

    for (int j=0; j<BUFFER_SIZE; ++j)
        // data[rec_id].at(idx + j) = monitor[j];    
        data[rec_id][idx + j] = monitor[j];    
}

void Channel::dub(float monitor[BUFFER_SIZE], int idx){

    for (int j=0, i=idx; i < idx+BUFFER_SIZE; j++,++i){
        // data[rec_id].at(i) += monitor[j] + data[play_id].at(i); // overwrite data[play_id] data to data[rec_id]
        data[rec_id][i] += monitor[j] + data[play_id][i];
    }
}

void Channel::undub(){
// Toggled play_id/rec_id. If pressed twice, then return to the same condition, that is -> do not undub at all.
    num_tracks = states_avail[0];
    states_avail[0] = states_avail[1];
    states_avail[1] = num_tracks;
    
    play_id = states_avail[0]%num_states;
    rec_id = states_avail[1]%num_states;
}

void Channel::clean(){      
    for (int state = 0; state<num_states; ++state){
        // data[state].clear();
        // data[state].erase(data[state].begin(),data[state].end());
        std::fill(data[state].begin(), data[state].end(), 0);
        states_avail[state] = 0;
    }
    rec_id = play_id = 0;
    num_tracks = 0;
}

std::vector<float> Channel::get_out_signal(int idx){
    std::vector<float> curr(data[play_id].begin() + idx, data[play_id].begin() + idx + BUFFER_SIZE);
    return  curr;
}

void Channel::reset_num_tracks(){
    num_tracks = 0;   
}

void Channel::add_track(){
    states_avail[0] = num_tracks; // previous state
    ++num_tracks;    
    states_avail[1] = num_tracks; // current state
    play_id = states_avail[0]%num_states;
    rec_id = states_avail[1]%num_states;
}

int Channel::get_num_tracks(){
    return num_tracks;
}

float Channel::get_volume(){
    return (volume)/100.;
}

void Channel::set_volume(int volume){
    this->volume = volume;
}

void Channel::volume_up(){
    volume += volume_step;
    if (volume > 100)
        volume = 100;
    std::cout<<"Channel::volume_up volume --> "<<volume<<std::endl;
}

void Channel::volume_down(){
    volume -= volume_step;
    if (volume < 0)
        volume = 0;
    std::cout<<"Channel::volume_down volume --> "<<volume<<std::endl;
}

bool Channel::isEmpty(){
    return num_tracks == 0;
}
