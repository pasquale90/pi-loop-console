#!bin/bash
#cat /proc/asound/cards

jackd -R -v -t10000 -dalsa -d"hw:1" -r16000 -p512
