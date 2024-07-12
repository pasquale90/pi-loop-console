#ifndef RESPONSE_H
#define RESPONSE_H

#include <atomic>

/*! @brief Message enumerator*/
enum Message : signed short{
    // PLAY=1, // blink G
    // PAUSE, // set on/off G
    // ERASE,   // blink R,G
    // REC,     // set on/off R
    // UNDUB,    // blink R
    // RESET,  // blink all channels
    LOOPER=1,
    JAM_SAVED,
    ARM,
    MNTR,
    EFF,
    SESSION_CHANGE,
    SHUTDOWN,
    NUM_MESSAGES,
    EMPTY=0
};

/*! @brief Stores the looper state.
*/
struct LooperState{
    std::atomic<bool> playbacks[3];
    std::atomic<bool> record[3];
    std::atomic<int> num_tracks[3];
    std::atomic<int> is_changed;
    
    /*! @brief Default constructor
    */
    LooperState(){
        reset();
    }

    /*! @brief Copy constructor
    */
    LooperState(const LooperState& obj){
        for (int i=0; i<3; ++i){
            playbacks[i].store(obj.playbacks[i].load());
            record[i].store(obj.record[i].load());
            num_tracks[i].store(obj.num_tracks[i].load());
    	}
        is_changed.store(obj.is_changed.load());
    }

    /*! @brief Assignment operator
    */
    LooperState& operator=(const LooperState& obj){
        if (this == &obj)
            return *this;
        for (int i=0; i<3; ++i){
            playbacks[i].store(obj.playbacks[i].load());
            record[i].store(obj.record[i].load());
            num_tracks[i].store(obj.num_tracks[i].load());
    	}
        is_changed.store(obj.is_changed.load());
        return *this;
    }

    /*! @brief Reset function resets the object member variables to the default values.
    */
    void reset(){
        for (int i=0; i<3; ++i){
            playbacks[i].store(false);
            record[i].store(false);
            num_tracks[i].store(0);
        }
        is_changed.store(false);
    }
};

/*! @brief Defines a response message destinated to the displayer module of the program
*/
struct Response{
    /*! @brief Default constructor
    */
    Response(){
        holder.store(-1); //zero initialized
        msg.store(EMPTY);
        value.store(-1);
    }
    
    /*! @brief Copy constructor
    */
    Response(const Response& obj){
        holder.store(obj.holder); //zero initialized
        msg.store(obj.msg);
        value.store(obj.value);
        looper_state = obj.looper_state;
    }

    /*! @brief Assignment operator
    */
    Response& operator=(const Response& obj){
        if (this == &obj)
            return *this;
        
        holder.store(obj.holder); //zero initialized
        msg.store(obj.msg);
        value.store(obj.value);
        looper_state = obj.looper_state;

        return *this;
    }

    std::atomic<int> holder {-1}; //zero initialized
    std::atomic<Message> msg {EMPTY};
    std::atomic<int> value {-1};
    LooperState looper_state;

    /*! @brief Function that returns if a Response message is empty
     * @return bool - True if is empty (if msg ==  EMPTY or if looper_state is not changed). Otherwise false
    */
    bool isEmpty(){return msg.load()==EMPTY && !looper_state.is_changed.load();}

    /*! @brief Reset function resets the object member variables to the default values.
     * @return void
    */
    void reset(){
        holder.store(-1);
        msg.store(EMPTY);
        value.store(-1);
        looper_state.reset();
    }
};



#endif