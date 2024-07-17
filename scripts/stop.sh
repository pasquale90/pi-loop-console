#!bin/bash

kill -9 $(pgrep -x piloop)
sudo killall -9 jackd

