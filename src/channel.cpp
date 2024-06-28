#include "channel.h"
#include <iostream>

Channel::Channel() {
    rec_id = 0;
    play_id = 0;
    num_tracks = 0; 
    for (int state = 0; state<num_states; ++state){
        states_avail[state] = 0;
    }

    loop_length.store(0);

// @TODO apply a method to retrieve the current volume set on corresponding potentiometer
    volume = 70;
    volume_step = volumeStep;
}

void Channel::reset(){

    // std::cout<<"Resetting channel "<<channel_name<<std::endl;
    loop_length.store(0);

    for (int state = 0; state<num_states; ++state){
        data[state].clear();
        data[state].shrink_to_fit();
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

// @TODO let Looper class handle those cases. Take care of atomic operations.
    bool looper_initialized = loop_length.load();   
    if ( looper_initialized){
        if( idx<loop_length.load()-BUFFER_SIZE-1 ){
            for (int j=0; j<BUFFER_SIZE; ++j){
                data[rec_id][idx++] += monitor[j];
                // ++idx;
            }
        }
    }else{
        for (int j=0; j<BUFFER_SIZE; ++j)
            data[rec_id].push_back(monitor[j]);
    }
    
}

void Channel::dub(float monitor[BUFFER_SIZE], int idx){

    for (int j=0, i=idx; i < idx+BUFFER_SIZE; j++,++i){
        data[rec_id].at(i) += monitor[j] + data[play_id].at(i); // overwrite data[play_id] data to data[rec_id]
    }
}

void Channel::undub(){
// Toggled play_id/rec_id. If pressed twice, then return to the same condition, that is -> do not undub at all.
    num_tracks = states_avail[0];
    states_avail[0] = states_avail[1];
    states_avail[1] = num_tracks;
    
    play_id = states_avail[0]%num_states;
    rec_id = states_avail[1]%num_states;
    // play_id = rec_id;
    // rec_id = int(!(bool)rec_id);
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

// float* Channel::get_out_signal(int idx){    
std::vector<float> Channel::get_out_signal(int idx){
    std::vector<float> curr(data[play_id].begin() + idx, data[play_id].begin() + idx + BUFFER_SIZE);
    return  curr;
}

void Channel::set_loop_length(bool shrink2fit,int loopLength){

    loop_length.store(loopLength);

    //  if (shrink2fit){
    //     data[0].shrink_to_fit();
    //     data[1].reserve(loopLength);
    //     data[1].resize(loopLength,0);
    // }
    // else{
    //     data[0].reserve(loopLength);
    //     data[1].reserve(loopLength);
    //     data[0].resize(loopLength,0);
    //     data[1].resize(loopLength,0);
    // }

    int ll = loop_length.load();
    if (shrink2fit){
        // std::cout<<"\n\n\t\t BEFORE DEBUG CAPACITY ll==data[0].size()==data[0].capacity() ---> "<<ll<<"=="<<data[0].size()<<"=="<<data[0].capacity()<<"\n\n"<<std::endl;
        data[0].erase(data[0].begin()+ll,data[0].end());
        data[0].resize(ll);
        data[0].shrink_to_fit();
        // std::cout<<"\n\n\t\t AFTER DEBUG CAPACITY ll==data[0].size()==data[0].capacity() ---> "<<ll<<"=="<<data[0].size()<<"=="<<data[0].capacity()<<"\n\n"<<std::endl;
        
        data[1].reserve(ll);
        data[1].resize(ll,0);
        // std::cout<<"\n\n\t\t AFTER DEBUG CAPACITY ll==data[1].size()==data[1].capacity() ---> "<<ll<<"=="<<data[1].size()<<"=="<<data[1].capacity()<<"\n\n"<<std::endl;
    }
    else{
        data[0].reserve(ll);
        data[1].reserve(ll);
        data[0].resize(ll,0);
        data[1].resize(ll,0);
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

// void Channel::debug(){
//     // std::cout<<"Channel "<<channel_name<<"rec_id/play_id = "<<rec_id<<"/"<<play_id<<std::endl;    
//     // if (num_tracks)
//         // std::cout<<"Channel "<<channel_name<<"::data[play_id/rec_id][0] = "<<data[play_id][0]<<"/"<<data[rec_id][0]<<std::endl;
//         std::cout<<"Channel "<<channel_name<<" data[rec_id/play_id].size() = "<<data[rec_id].size()<<"/"<<data[play_id].size()<<"(" <<data[rec_id].capacity()<<"/"<<data[play_id].capacity()<<")           " <<" states_avail "<< states_avail[0]<<","<<states_avail[1]<<" loop_length="<<loop_length.load()<<std::endl;    
// }
