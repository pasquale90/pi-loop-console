#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED

//enumerator
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
    NUM_CONTROLS
};

#endif