COMPILER :=g++
OPTIONS :=-g -pedantic -Wall -Wextra -Werror
NOPTIONS :=-g -pedantic -Wall -Wno-extra # -Werror
COMPILE :=$(COMPILER) $(NOPTIONS)

#start_jack:
#	$(COMPILE) start_jack.o -Iusr/include -ljack -o start_jack

start_jack:start_jack.cpp jack/jack.h jack/control.h
	$(COMPILER) $(NOPTIONS) start_jack.cpp -I. -Lusr/lib/aarch64-linux-gnu -ljack -ljackserver -ljacknet -o server_control

clean:
	rm -f start_jack.o start_jack
















