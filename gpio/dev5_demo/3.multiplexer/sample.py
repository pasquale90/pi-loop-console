from machine import Pin  
from time import sleep 

select_pin_nums = [27,17,10,22] 
s_pins = [Pin(i, Pin.OUT) for i in select_pin_nums] 
#z_pin = Pin(7, Pin.OUT) 
 
def select_pin(p, pins): 
    for i in range(3): 
        pins[i].value((p>>i)&1)  

while True: 
    for i in range(8): 
        select_pin(i, s_pins) 
 #       z_pin.value(1) 
        sleep(0.25) 
  #      z_pin.value(0) 
   #     sleep(0.25)
