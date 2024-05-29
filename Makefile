COMPILER :=g++ -std=c++11
OPTIONS :=-g -pedantic -Wall -Wextra -Werror
NOPTIONS :=-g -pedantic -Wall -Wno-extra # -Werror
CFLAGS=-D DEV -D K6 #DEV/REL K6/PCH/CODEC
COMPILE :=$(COMPILER) $(NOPTIONS) $(CFLAGS)
INCLUDE :=-I./include
OBJECTS := build/channel.o build/looper.o build/mixer.o build/monitor.o build/effects.o build/audioserver.o build/handshake.o build/session.o build/keyboard.o build/interface.o build/config.o build/menu.o build/piloop.o build/main.o

JSONCPP :=-I/usr/include/jsoncpp -L/usr/lib/x86_64-linux-gnu -ljsoncpp
EVDEV :=-I/usr/include -levdev
JACK :=-I/usr/include -L/usr/lib -ljack -ljackserver -ljacknet

all:piloop

piloop:$(OBJECTS)
	$(COMPILE) $(OBJECTS) $(JSONCPP) $(EVDEV)  $(JACK) -o piloop -ljsoncpp -lpthread

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
	$(COMPILE) -c src/mixer.cpp $(INCLUDE) $(JSONCPP) -o build/mixer.o

build/looper.o:src/looper.cpp include/looper.h
	$(COMPILE) -c src/looper.cpp $(INCLUDE) $(JSONCPP) -o build/looper.o

build/channel.o:src/channel.cpp include/channel.h
	$(COMPILE) -c src/channel.cpp $(INCLUDE) -o build/channel.o

build/session.o:src/session.cpp include/session.h
	$(COMPILE) -c src/session.cpp $(INCLUDE) $(JSONCPP) -o build/session.o

build/keyboard.o:src/keyboard.cpp include/keyboard.h
	$(COMPILE) -c src/keyboard.cpp $(INCLUDE) $(EVDEV) -o build/keyboard.o

build/interface.o:src/interface.cpp include/interface.h
	$(COMPILE) -c src/interface.cpp $(INCLUDE) -o build/interface.o
	
build/config.o:src/config.cpp include/config.h
	$(COMPILE) -c src/config.cpp $(INCLUDE) $(JSONCPP) -o build/config.o
	
build/menu.o:src/menu.cpp include/menu.h src/config.cpp include/config.h
	$(COMPILE) -c src/menu.cpp $(INCLUDE) $(JSONCPP) -o build/menu.o

build/piloop.o: include/piloop.h src/piloop.cpp
	$(COMPILE) -c src/piloop.cpp $(INCLUDE) $(JSONCPP) $(EVDEV) -o build/piloop.o -lpthread

build/main.o:src/main.cpp
	$(COMPILE) -c src/main.cpp $(INCLUDE) $(JSONCPP) $(EVDEV) -o build/main.o -lpthread

clean:
	rm -rf build/* logs/* piloop
