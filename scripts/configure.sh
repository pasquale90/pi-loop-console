#!bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR && cd ..
source PILOOP_SETTINGS

# if KEYBOARD_DEVICE exists else, set value to it.
if [[ -z "${KEYBOARD_DEVICE}" ]];then 
    # detect keyboard device..
    kbdEvents=($(ls /dev/input/by-path | grep "event-kbd"))  
    echo ${kbdEvents}
    for forCounter in "${kbdEvents[@]}"
    do
        KEYBOARD_DEVICE=$(readlink --canonicalize "/dev/input/by-path/${forCounter}")     
    done
    echo ${KEYBOARD_DEVICE}
fi

# same with i2c bus
if [[ -z "${I2C_BUS_ADDR}" ]];then 
    echo "i have to find it."
    I2C_BUS_ADDR="0x20" # known value -> FIX
fi

export PILOOP_MODE
export AUDIO_DEVICE
export SAMPLE_RATE
export BUFFER_SIZE
export COMPILATION_TOOL
export KEYBOARD_DEVICE
export I2C_BUS_ADDR

if [ ! -d "build" ]; then 
    mkdir build 
fi
cd build
cmake ..
cd ..