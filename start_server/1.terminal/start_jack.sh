#!bin/bash
#cat /proc/asound/cards

#jackd --verbose -R -v -t10000 -dalsa -d"hw:K6" -r16000 -p512
jackd --realtime --verbose -t 10000 -d alsa -d hw:K6 -r 16000 -p 512
#jackd --realtime --verbose -t10000 -dalsa -dhw:K6 -r44100 -p128

#for debug
#tail -f ~/.log/jack/jackdbus.log

#jack_control stop
#killall jackd
#sudo killall -9 jackdbus
#sudo killall -9 jackd
#jack_control exit
#jack_control start
