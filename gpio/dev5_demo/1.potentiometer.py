# https://www.ardumotive.com/how-to-use-a-potentiometeren.html

#Libraries
import RPi.GPIO as GPIO
from time import sleep
#Set warnings off (optional)
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
#Set Button and LED pins

#GPIO ATTEMPT
Buttons= [13,12,20]
LEDS = [24,23,18]

#Setup Button and LED
for i in range(3):

  GPIO.setup(Buttons[i],GPIO.IN,pull_up_down=GPIO.PUD_UP)

  GPIO.setup(LEDS[i],GPIO.OUT)
#flag = 0

while True:

    for i in range(3):
        button_state = GPIO.input(Buttons[i])
        print(button_state)
        if button_state != 0:
            GPIO.output(LEDS[i], GPIO.HIGH)
        else:
            GPIO.output(LEDS[i], GPIO.LOW)

    sleep(1)
    '''
    if button_state==0:
        sleep(0.5)
        if flag==0:
            flag=1
        else:
            flag=0
    if flag==1:
        GPIO.output(LED,GPIO.HIGH)
    else:
        GPIO.output(LED,GPIO.LOW)
    '''
