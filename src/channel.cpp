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
    currSize = 0;
}

void Channel::update_signal(float*[F_NUM_INPUTS]){
    // https://www.gnu.org/software/libc/manual/html_node/Alloca-Example.html
}

void Channel::recdub(){}
void Channel::undub(){}
void Channel::pause(){}
void Channel::reset(){}

