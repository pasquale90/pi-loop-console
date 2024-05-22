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

if [[ " $@ " =~ "e" ]]; then
    echo ${sudopass} | sudo -S -k ./piloop
    # echo ${sudopass} | sudo -S -k ./piloop &> logs/piloop.log &
fi

