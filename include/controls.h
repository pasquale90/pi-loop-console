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

// inline const char* ToString(Control v)
// {
//     switch (v)
//     {
//         case CH1_RECDUB:   return "CH1_RECDUB";
//         case CH1_STOP:     return "CH1_STOP";
//         case CH2_RECDUB:   return "CH2_RECDUB";
//         case CH2_STOP:     return "CH2_STOP";
//         case CH3_RECDUB:   return "CH3_RECDUB";
//         case CH3_STOP:     return "CH3_STOP";
//         case START_ALL:    return "START_ALL";
//         case SAVE_JAM:     return "SAVE_JAM";
//         case CH1_ARM:      return "CH1_ARM";
//         case CH1_MNTR:     return "CH1_MNTR";
//         case CH2_ARM:      return "CH2_ARM";
//         case CH2_MNTR:     return "CH2_MNTR";
//         case CH1_EFF1:     return "CH1_EFF1";
//         case CH1_EFF2:     return "CH1_EFF2";
//         case CH1_EFF3:     return "CH1_EFF3";
//         case CH2_EFF1:     return "CH2_EFF1";
//         case CH2_EFF2:     return "CH2_EFF2";
//         case CH2_EFF3:     return "CH2_EFF3";
//         default:           return "Unknown";
//     }
// }

#endif