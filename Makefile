COMPILER :=g++
OPTIONS :=-g -pedantic -Wall -Wextra -Werror
NOPTIONS :=-g -pedantic -Wall -Wno-extra # -Werror
COMPILE :=$(COMPILER) $(NOPTIONS)
OBJECTS :=build/handshake.o build/route.o build/channel.o build/session.o build/main.o

# all:piloop

piloop:$(OBJECTS)
	$(COMPILE) $(OBJECTS) -ljack -o piloop


build/handshake.o:src/handshake.cpp libs/jack/jack.h libs/jack/types.h
	$(COMPILER) $(NOPTIONS) -c src/handshake.cpp  -Ilibs -ljack -o build/handshake.o

build/route.o:src/route.cpp src/handshake.h
	$(COMPILE) -c src/route.cpp -I. -o build/route.o

build/channel.o:src/channel.cpp src/handshake.h src/route.h
	$(COMPILE) -c src/channel.cpp -I. -o build/channel.o

build/session.o:src/session.cpp src/channel.h
	$(COMPILE) -c src/session.cpp -I. -o build/session.o

build/main.o:src/main.cpp src/session.h src/handshake.h src/channel.h
	$(COMPILE) -c src/main.cpp -I. -o build/main.o

clean:
	rm -f build/*.o
















