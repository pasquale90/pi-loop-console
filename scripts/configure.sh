#!bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR && cd ..
source PILOOP_SETTINGS

# if KEYBOARD_DEVICE exists else, set value to it.
if [ "${PILOOP_MODE}" = "PC" ];then
    if [[ -z "${KEYBOARD_DEVICE}" ]];then 
        # detect keyboard device..
        kbdEvents=($(ls /dev/input/by-path | grep "event-kbd"))  
        # echo ${kbdEvents}
        for forCounter in "${kbdEvents[@]}"
        do
            KEYBOARD_DEVICE=$(readlink --canonicalize "/dev/input/by-path/${forCounter}")     
        done
        # echo ${KEYBOARD_DEVICE}
    fi
fi

# same with i2c bus
if [ "${PILOOP_MODE}" = "RPI" ];then
    if [[ -z "${I2C_BUS_ADDR}" ]];then 
        # find the i2c bus address. Parse i2c detect -y 1 output
        i2c_out=`i2cdetect -y 1`
        for (( i=0; i<${#i2c_out}; i++ )); do
            curr=${i2c_out:$i:1}
            next=${i2c_out:$i+1:1}
            nextnext=${i2c_out:$i+2:1}
            if [[ $curr =~ ^[0-9]+$ ]] && [[ $next =~ ^[0-9]+$ ]] && [[ $nextnext != ':' ]]; then
                echo "$curr-$next"
                break
            fi
        done
        I2C_BUS_ADDR="${curr}${next}"
        echo "$I2C_BUS_ADDR"

        if ! [[ $I2C_BUS_ADDR =~ ^[0-9]+$ ]];then
            printf "\nError:i2c address not found! Run (with sudo):\n $ i2cdetect -y 1\n... to inspect, and set in PILOOP_SETTINGS manually\n\n"
            exit -1
        fi
    fi
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