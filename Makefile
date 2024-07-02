COMPILER :=g++ -std=c++11
OPTIONS :=-g -pedantic -Wall -Wextra -Werror
NOPTIONS :=-g -pedantic -Wall -Wno-extra # -Werror
MODE :=REL
CFLAGS=-D $(MODE) -D K6 #DEV/REL K6/PCH/CODEC
COMPILE :=$(COMPILER) $(NOPTIONS) $(CFLAGS)
INCLUDE :=-I./include
OBJECTS := build/metronome.o build/channel.o build/looper.o build/mixer.o build/monitor.o build/effects.o build/audioserver.o build/handshake.o build/session.o  build/interface.o build/config.o build/menu.o build/piloop.o build/main.o 

ifeq ($(MODE), DEV)
	OBJECTS += build/keyboard.o
else ifeq ($(MODE), REL)
	OBJECTS += build/buttons.o 
	OBJECTS += build/leds.o 
	INCLUDE += -I./include/gpio
else
	$(error MODE variable is set with wrong value. Set as REL for release and DEV for dev)
endif

WIRINGPI :=-L/usr/lib -lwiringPi
AUDIOFILE :=-I./external
JSONCPP :=-I/usr/include/jsoncpp -L/usr/lib/x86_64-linux-gnu -ljsoncpp
EVDEV :=-I/usr/include -levdev
JACK :=-I/usr/include -L/usr/lib -ljack -ljackserver -ljacknet

all:piloop

piloop:$(OBJECTS)
	$(COMPILE) $(OBJECTS) $(JSONCPP) $(EVDEV)  $(JACK) $(WIRINGPI) -o piloop -ljsoncpp -lpthread

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
build/keyboard.o: src/keyboard.cpp include/keyboard.h
	$(COMPILE) -c src/keyboard.cpp $(INCLUDE) $(EVDEV) -o build/keyboard.o
else ifeq ($(MODE), REL)
build/buttons.o: src/gpio/buttons.cpp include/gpio/buttons.h	
	$(COMPILE) -c src/gpio/buttons.cpp $(INCLUDE) $(WIRINGPI) -o build/buttons.o
build/leds.o: src/gpio/leds.cpp include/gpio/leds.h	
	$(COMPILE) -c src/gpio/leds.cpp $(INCLUDE) $(WIRINGPI) -o build/leds.o
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
	$(COMPILE) -c src/piloop.cpp $(INCLUDE) $(JSONCPP) $(EVDEV) $(AUDIOFILE) -o build/piloop.o -lpthread

build/main.o:src/main.cpp
	$(COMPILE) -c src/main.cpp $(INCLUDE) $(JSONCPP) $(EVDEV) $(AUDIOFILE) -o build/main.o -lpthread

clean:
	rm -rf build/* logs/* piloop