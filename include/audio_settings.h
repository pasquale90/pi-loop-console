#ifndef AUDIO_SETTINGS_H
#define AUDIO_SETTINGS_H

    // #define BUFFER_SIZE 256
    // #define SAMPLE_RATE 44100
    #define BUFFER_SIZE 512
    #define SAMPLE_RATE 16000
    #define BIT_QUANTIZATION 24

    #ifdef K6
        #define DEVICE_ID "K6"
        #define DEV_INFO "Komplete Audio 6"
        #define INST 1
        #define MIC 1
        #define PHONES 1
        #define STEREO_OUT 1
        #define SUB_OUT 1
    #elif CODEC
        #define DEVICE_ID "CODEC"
        #define DEV_INFO "Beringher UCA202"
        #define INST 0
        #define MIC 1
        #define PHONES 1
        #define STEREO_OUT 1
        #define SUB_OUT 0
    #elif PCH
        #define DEVICE_ID "PCH"
        #define DEV_INFO "build-in speakers"
        #define INST 0
        #define MIC 1
        #define PHONES 0
        #define STEREO_OUT 1
        #define SUB_OUT 0
    #endif

    #define NUM_INPUTS MIC+INST
    #define TOTAL_OUTPUTS 2 * STEREO_OUT + 2 * SUB_OUT + PHONES

// define 2 maximum inputs (mic+inst / mic only) and 2 maximum outputs (stereo/mono)
//@TODO support phones and sub_out 

    #define MAX_INPUTS 2
    #define MAX_OUTPUTS 2

    #if NUM_INPUTS == 1
        #define F_NUM_INPUTS 1
    #else
        #define F_NUM_INPUTS MAX_INPUTS
    #endif

    #if NUM_OUTPUTS == 1
        #define F_NUM_OUTPUTS 1
    #else
        #define F_NUM_OUTPUTS MAX_OUTPUTS
    #endif

#endif
