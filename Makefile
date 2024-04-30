COMPILER :=g++
OPTIONS :=-g -pedantic -Wall -Wextra -Werror
NOPTIONS :=-g -pedantic -Wall -Wno-extra # -Werror
CFLAGS=-D DEV #DEV/REL
COMPILE :=$(COMPILER) $(NOPTIONS) $(CFLAGS)
# OBJECTS :=build/handshake.o build/route.o build/channel.o build/session.o build/main.o
OBJECTS :=build/keyboard.o build/interface.o build/config.o build/menu.o build/main.o

JSONCPP :=-I/usr/include/jsoncpp -L/usr/lib/x86_64-linux-gnu -ljsoncpp

EVDEV :=-I/usr/include -levdev #-L/usr/lib/x86_64-linux-gnu/

# JACK :=-I/usr/include -L/usr/lib64/pipewire-0.3 -ljack -ljackserver

all:piloop

piloop:$(OBJECTS)
	$(COMPILE) $(OBJECTS) $(JSONCPP) $(EVDEV) -o piloop -ljsoncpp -lpthread

# all:piloop

# piloop:$(OBJECTS)
# 	$(COMPILE) $(OBJECTS) $(JACK) -o piloop

# build/handshake.o:src/handshake.cpp
# 	$(COMPILER) $(NOPTIONS) -c src/handshake.cpp -I/usr/include $(JACK) -o build/handshake.o

# build/route.o:src/route.cpp src/handshake.h
# 	$(COMPILE) -c src/route.cpp -o build/route.o

# build/channel.o:src/channel.cpp src/handshake.h src/route.h
# 	$(COMPILE) -c src/channel.cpp -o build/channel.o

# build/session.o:src/session.cpp src/channel.h
# 	$(COMPILE) -c src/session.cpp -o build/session.o

build/keyboard.o:src/keyboard.cpp include/keyboard.h
	$(COMPILE) -c src/keyboard.cpp -I./include $(EVDEV) -o build/keyboard.o

build/interface.o:src/interface.cpp include/interface.h
	$(COMPILE) -c src/interface.cpp -I./include -o build/interface.o
	
build/config.o:src/config.cpp include/config.h
	$(COMPILE) -c src/config.cpp -I./include $(JSONCPP) -o build/config.o
	
build/menu.o:src/menu.cpp include/menu.h src/config.cpp include/config.h
	$(COMPILE) -c src/menu.cpp -I./include $(JSONCPP) -o build/menu.o
	
build/main.o:src/main.cpp
	$(COMPILE) -c src/main.cpp -I./include/ $(JSONCPP) $(EVDEV) -o build/main.o -lpthread

clean:
	rm -rf build/* logs/* piloop
