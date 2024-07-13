COMPILER :=g++ -std=c++11
OPTIONS :=-g -pedantic -Wall -Wextra -Werror
NOPTIONS :=-g -pedantic -Wall -Wno-extra # -Werror
MODE := DEV
AUDIO_INTERFACE := PCH
CFLAGS=-D $(MODE) -D $(AUDIO_INTERFACE) #DEV/REL K6/PCH/CODEC
COMPILE :=$(COMPILER) $(NOPTIONS) $(CFLAGS)
INCLUDE :=-I./include
OBJECTS := build/metronome.o build/channel.o build/looper.o build/mixer.o build/monitor.o build/effects.o build/audioserver.o build/handshake.o build/session.o  build/interface.o build/config.o build/menu.o build/piloop.o build/main.o 

ifeq ($(MODE), DEV)
	OBJECTS += build/keyboard.o build/screen.o build/computer.o
	INCLUDE += -I./include/computer
	EVDEV :=-I/usr/include -levdev
	CONDITIONAL_LIB = $(EVDEV)
else ifeq ($(MODE), REL)
	OBJECTS += build/buttons.o build/potentiometers.o build/leds.o build/gpio.o
	INCLUDE += -I./include/gpio
	WIRINGPI :=-L/usr/lib -lwiringPi
	CONDITIONAL_LIB = $(WIRINGPI)
else
	$(error MODE variable is set with wrong value. Set as REL for release and DEV for dev)
endif


# make a var holding EVDEV | WIRINGPI
AUDIOFILE :=-I./external
JSONCPP :=-I/usr/include/jsoncpp -L/usr/lib/x86_64-linux-gnu -ljsoncpp
JACK :=-I/usr/include -L/usr/lib -ljack -ljackserver -ljacknet

all:piloop

piloop:$(OBJECTS)
	$(COMPILE) $(OBJECTS) $(JSONCPP) $(CONDITIONAL_LIB)  $(JACK) $(WIRINGPI) -o piloop -ljsoncpp -lpthread

build/audioserver.o: include/audioserver.h src/audioserver.cpp
	$(COMPILE) -c src/audioserver.cpp $(INCLUDE) $(JACK) $(JSONCPP) -o build/audioserver.o

build/handshake.o:src/handshake.cpp include/handshake.h
	$(COMPILE) -c src/handshake.cpp $(INCLUDE) $(JACK) $(JSONCPP) -o build/handshake.o

build/monitor.o:src/monitor.cpp include/monitor.h
	$(COMPILE) -c src/monitor.cpp $(INCLUDE) $(JSONCPP) -o build/monitor.o

build/effects.o:src/effects.cpp include/effects.h
	$(COMPILE) -c src/effects.cpp $(INCLUDE) -o build/effects.o

build/channel.o:src/channel.cpp include/channel.h
	$(COMPILE) -c src/channel.cpp $(INCLUDE) -o build/channel.o

build/mixer.o:src/mixer.cpp include/mixer.h
	$(COMPILE) -c src/mixer.cpp $(INCLUDE) $(AUDIOFILE) -o build/mixer.o 

build/looper.o:src/looper.cpp include/looper.h
	$(COMPILE) -c src/looper.cpp $(INCLUDE) $(JSONCPP) -o build/looper.o

build/metronome.o:src/metronome.cpp include/metronome.h
	$(COMPILE) -c src/metronome.cpp $(INCLUDE) $(JSONCPP) -o build/metronome.o

build/session.o:src/session.cpp include/session.h
	$(COMPILE) -c src/session.cpp $(INCLUDE) $(JSONCPP) $(AUDIOFILE) -o build/session.o

ifeq ($(MODE), DEV)

build/computer.o: src/computer.cpp include/computer.h
	$(COMPILE) -c src/computer.cpp $(INCLUDE) $(EVDEV) -o build/computer.o

build/keyboard.o: src/computer/keyboard.cpp include/computer/keyboard.h
	$(COMPILE) -c src/computer/keyboard.cpp $(INCLUDE) $(EVDEV) -o build/keyboard.o

build/screen.o: src/computer/screen.cpp include/computer/screen.h
	$(COMPILE) -c src/computer/screen.cpp $(INCLUDE) -o build/screen.o

else ifeq ($(MODE), REL)

build/gpio.o: src/gpio.cpp include/gpio.h	
	$(COMPILE) -c src/gpio.cpp $(INCLUDE) $(WIRINGPI) -o build/gpio.o

build/buttons.o: src/gpio/buttons.cpp include/gpio/buttons.h	
	$(COMPILE) -c src/gpio/buttons.cpp $(INCLUDE) $(WIRINGPI) -o build/buttons.o

build/leds.o: src/gpio/leds.cpp include/gpio/leds.h	
	$(COMPILE) -c src/gpio/leds.cpp $(INCLUDE) $(WIRINGPI) -o build/leds.o
	
build/potentiometers.o: src/gpio/potentiometers.cpp include/gpio/potentiometers.h	
	$(COMPILE) -c src/gpio/potentiometers.cpp $(INCLUDE) $(WIRINGPI) -o build/potentiometers.o
else
	$(error MODE variable is set with wrong value. Set as REL for release and DEV for dev)
endif

build/interface.o:src/interface.cpp include/interface.h
	$(COMPILE) -c src/interface.cpp $(INCLUDE) -o build/interface.o
	
build/config.o:src/config.cpp include/config.h
	$(COMPILE) -c src/config.cpp $(INCLUDE) $(JSONCPP) -o build/config.o
	
build/menu.o:src/menu.cpp include/menu.h src/config.cpp include/config.h
	$(COMPILE) -c src/menu.cpp $(INCLUDE) $(JSONCPP) $(AUDIOFILE) -o build/menu.o

build/piloop.o: include/piloop.h src/piloop.cpp
	$(COMPILE) -c src/piloop.cpp $(INCLUDE) $(JSONCPP) $(CONDITIONAL_LIB) $(AUDIOFILE) -o build/piloop.o -lpthread

build/main.o:src/main.cpp
	$(COMPILE) -c src/main.cpp $(INCLUDE) $(JSONCPP) $(CONDITIONAL_LIB) $(AUDIOFILE) -o build/main.o -lpthread

clean:
	rm -rf build/* logs/* piloop
