#!bin/bash

###### bash arguments ######
# b : build code
# s : server run
# e : code execution
############################

if [[ " $@ " =~ "b" ]]; then
    rm -r build piloop logs/*.log
    mkdir build
    cd build
    cmake ../
    cmake --build .
    mv piloop ../
    cd ../
fi

################################################################ ARGUMENTS ################################################################
#---------------------------------------------------------------- system -----------------------------------------------------------------#
audio_device=Generic_1
#----------------------------------------------------------------- audio -----------------------------------------------------------------#
sample_rate=22050 #44100 #16000 #
buffer_size=256 #512
bit_quantization=24
###########################################################################################################################################

# Preparing arguments for main
args=""
if [ ! -z ${1:-${sample_rate}} ]; then args="${args} --sample_rate ${sample_rate}"; fi
if [ ! -z ${2:-${buffer_size}} ]; then args="${args} --buffer_size ${buffer_size}"; fi
if [ ! -z ${3:-${bit_quantization}} ]; then args="${args} --bit_quantization ${bit_quantization}"; fi
echo "args --> ${args}"

if [[ " $@ " =~ "s" ]]; then
    # Creating command for JACK SERVER
    startJack_command="jackd --realtime --verbose -t 10000 -d alsa"
    startJack_command="${startJack_command} -d hw:${audio_device}"
    startJack_command="${startJack_command} -r ${sample_rate}"
    startJack_command="${startJack_command} -p ${buffer_size}"
    echo -e "\nrunning startJack_command >> \n${startJack_command}"

    # Applying sudo priviledges
    # # RUN SUDO INSTRUCTIONS - DEACTIVATED
    # export HISTIGNORE='*sudo -S*'
    # sudopass=${1}
    # if [ -z "$1" ]; then
    #     >&2 echo "No password provided"
    #     exit 1
    # fi

    # start_jackd
    # start_server="jackd --realtime --verbose -t 10000 -d alsa -d hw:K6 -r 16000 -p 512"
    # echo ${sudopass} | sudo -S -k nohup ${startJack_command} &> logs/audioServer.log & 

    nohup ${startJack_command} &> logs/server.log &
fi

sleep 1


# echo ${sudopass} | sudo -S -k ./demo ${sample_rate} ${buffer_size} &> logs/audio.log &
# ./piloop ${args} &> logs/audio.log &
if [[ " $@ " =~ "e" ]]; then
    ./piloop 
    echo -e "Run manually with :\n./piloop ${args}"
fi

