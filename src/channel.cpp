#include "channel.h"


// Channel::Channel(const char* chName):channel_name(chName) {
//     rec_id.store(0);
//     isempty.store(true);
//     start.store(false);
//     currSize = 0;
// }

Channel::Channel() {
    rec_id.store(0);
    isempty.store(true);
    start.store(false);
    loopLength = 0;
    loopIndex = 0;
}

void Channel::update_signal(float*[F_NUM_INPUTS]){
    // https://www.gnu.org/software/libc/manual/html_node/Alloca-Example.html

    // if no track on the channel recorded yet.
    
}

float** Channel::get_channel_buffer(){
    if (isempty.load()){
        return nullptr;
    }else{
        // return data
    }
}

void Channel::recdub(){

}
void Channel::undub(){

}
void Channel::pause(){

}
void Channel::reset(){
    
}

