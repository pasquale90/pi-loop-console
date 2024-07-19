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
        : '     # find the i2c bus address. Parse i2c detect -y 1 output
        i2c_out=`i2cdetect -y 1`
        n1=0
        n2=0
        b=0
        
        for (( i=0; i<${#i2c_out}; i++ )); do

            curr=${i2c_out:$i:1}
            next=${i2c_out:$i+1:1}

            if [  "$curr" = ' ' ] && [  "$next" = ' ' ];then 
                    b=0
            fi

            if [ "$b" = 0 ] && [ "$curr" = ':' ];then
                b=1
            fi

            if [ "$b" = 0 ] && [ cur!=" " ] || [ curr!="-" ];then
                if [ b==0 ] && [ $next != " " ] || [ $next != "-" ];then
                    n1=$(($curr + 1))
                    n2=$(($next + 1))
                fi
            fi
            # echo "${i2c_out:$i:1} ${i2c_out:$i+1:1}"
        done
        echo "$n1 $n2"
        '
        I2C_BUS_ADDR="0x20" # dumb value -> FIX LATTER
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