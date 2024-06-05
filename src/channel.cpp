#include "channel.h"
#include <iostream>

Channel::Channel() {
    rec_id = 0;
    num_tracks = 0; 
    for (int state = 0; state<num_states; ++state){
        states_avail[state] = 0;
    }
// @TODO apply a method to retrieve the current volume set on corresponding potentiometer
    volume = 70;
    volume_step = volumeStep;
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
        data[rec_id].push_back(monitor[j]);
}

void Channel::dub(float monitor[BUFFER_SIZE], int idx){

    for (int j=0, i=idx; i < idx+BUFFER_SIZE; j++,++i){
        data[rec_id].at(i) = monitor[j] + data[play_id].at(i); // overwrite data[play_id] data to data[rec_id]
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
        data[state].clear();
        states_avail[state] = 0;
    }
    rec_id = 0;
    num_tracks = 0; 
}

// float* Channel::get_out_signal(int idx){    
std::vector<float> Channel::get_out_signal(int idx){
    std::vector<float> curr(data[play_id].begin() + idx, data[play_id].begin() + idx + BUFFER_SIZE);
    return  curr;
}

void Channel::set_loop_length(bool shrink2fit,int loop_length){
    if (shrink2fit){
        data[0].shrink_to_fit();
        data[1].reserve(loop_length);
        data[1].resize(loop_length,0);
    }
    else{
        data[0].reserve(loop_length);
        data[1].reserve(loop_length);
        data[0].resize(loop_length,0);
        data[1].resize(loop_length,0);
    }
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
