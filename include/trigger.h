#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED

#include <atomic>

/*! @brief Control enumerator. Models all the controls that may be triggered by the user.
*/
enum Control: unsigned short{
    CH1_RECDUB=1,
    CH1_STOP,
    CH2_RECDUB,
    CH2_STOP,
    CH3_RECDUB,
    CH3_STOP,
    START_ALL,
    SAVE_JAM,
    IN1_ARM,
    IN1_MNTR,
    IN2_ARM,
    IN2_MNTR,
    IN1_EFF1,
    IN1_EFF2,
    IN1_EFF3,
    IN2_EFF1,
    IN2_EFF2,
    IN2_EFF3,
    TAP_TEMPO,
    PREV_SESSION,
    NEXT_SESSION,
    SAVE_SESSION, //==22
    CH1_VOL_LOW,
    CH1_VOL_HIGH,
    CH2_VOL_LOW,
    CH2_VOL_HIGH,
    CH3_VOL_LOW,
    CH3_VOL_HIGH,
    SHUTDOWN_PILOOP,
    NUM_CONTROLS,
    ISEMPTY=0
};

/*! @brief Holds the triggers of the user*/
struct Trigger{
    std::atomic<Control> control {ISEMPTY};
    std::atomic<int> subval {-1};

    /*! @brief Default constructor
    */
    Trigger(){
        control.store(ISEMPTY); //zero initialized
        subval.store(-1);
    }

    /*! @brief Copy constructor
    */
    Trigger(const Trigger& obj){
        control.store(obj.control); //zero initialized
        subval.store(obj.subval);
    }

    /*! @brief Assignment operator
    */
    Trigger& operator=(const Trigger& obj){
        if (this == &obj)
            return *this;
        control.store(obj.control); //zero initialized
        subval.store(obj.subval);
        return *this;
    }

    /*! @brief Reset function resets the object member variables to the default values.
     * @return void
    */
    void reset(){
        control.store(ISEMPTY);
        subval.store(-1);
    }
};

#endif